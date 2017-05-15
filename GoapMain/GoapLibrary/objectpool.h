/** @brief Implements the object pool pattern: A method for reusing
 * running objects of short life time.
 *
 * The ObjectPoolImpl reduces the memory management pressure by reducing
 * the amount of new and delete operations.
 *
 * @file objectpool.h
 */

#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#ifndef DefaultSharedPointer
#if defined(QT_CORE_LIB)
#include <QSharedPointer>
#define DefaultSharedPointer QSharedPointer
#else
#include <memory>
#define DefaultSharedPointer std::shared_ptr
#endif
#endif
#include <boost/lockfree/queue.hpp>
#include "shared.h"

/// The default max number of objects precreated in the pool.
#define DEFAULT_POOL_SIZE 512

namespace goap
{

/**
 * @brief HAS_MEMBER(void, clear(), void__clear);
 * Helper to know if a class implements a public `void clear()` function.
 */
HAS_MEMBER(void, clear(), void__clear);

/**
 * @brief The ObjectPoolImpl class
 * Using Pools gives you more control over how memory is used in your program.
 *
 * Pools are generally used when there is a lot of allocation and deallocation
 * of small objects. Instead of creating objects the usual way (by using
 * `new`) you ask the pool for the object creation. If the pool has precreated
 * objects, it will give you one of them. When the object is no needed anymore
 * it is recycled to the object pool. If the object is refered by a
 * smart pointer the recycling process is transparent.
 *
 * In general, use Pools when you need a more efficient way to do unusual
 * memory control.
 */
template <typename T, size_t Size = DEFAULT_POOL_SIZE>
class ObjectPoolImpl : public IFactoryItem
{
public:
    enum State
    {
        STATE_NULL,
        STATE_ON,
        STATE_OFF
    };

protected:
    /// The main and only data member: a fixed size lock free queue.
    boost::lockfree::queue<T *, boost::lockfree::capacity<Size>> _pool;
    State _state = STATE_ON;

    /**
     * @brief clearObject
     * Helper object for calling clear() method if available.
     * @param obj
     */
    template<typename C = T>
    typename std::enable_if<has_member_void__clear<C>::value, void>::type
    clearObject(T *obj)
    {
        return obj->clear();
    }

    template<typename C = T>
    typename std::enable_if < !has_member_void__clear<C>::value, void >::type
    clearObject(T *)
    {} // No clear object contents

    /**
     * @brief The SuicideToPool class
     * Wrapper class for reimplementing the suicide() function
     * for calling this pool recycler
     */
    template<typename C = T>
    class SuicideToPool : public C
    {
        //void innerSuicider
        template<class P>
        static auto innerSuicider(P *t, int) -> decltype((&P::suicide)(), void())
        {
            P::suicide();
        }
        template<class P>
        static void innerSuicider(P *t, long)
        {
            PrivateFactory::instanceDeleter(t, 0);
        }

    public:
        virtual ~SuicideToPool() = default;
        /// Intercepts the suicide() virtual function to recycle the object
        /// to the pool instead of 'delete' it.
        virtual void suicide()
        {
            auto pool = ObjectPoolImpl<T, Size>::singleton();
            bool bRec = pool->recycle(this);
            if (!bRec)
            {
                innerSuicider(static_cast<C *>(this), 0);
            }
            else
            {
                /// TODO: Use the logger
                //qDebug() << "Recycled " << endl;
            }
        }
    };

    ObjectPoolImpl(const ObjectPoolImpl<T, Size> &); // Disable copy contructor

    /**
     * @brief ObjectPoolImpl
     * You must create pool instances using ObjectPoolImpl<T, Size>::singleton()
     */
    ObjectPoolImpl() {}

public:
    typedef T value_type;

    virtual ~ObjectPoolImpl()
    {
        _state = STATE_OFF;
        clear();
    }

    /**
     * @brief clear
     * Clears (empties) the pool.
     * @return The number of objects released.
     */
    size_t clear()
    {
        size_t nObj = 0;
        T *obj = nullptr;
        while (_pool.pop(obj))
        {
            PrivateFactory::instanceDeleter(obj, 0);
            ++nObj;
        }
        return nObj;
    }

    /**
     * @brief empty
     * Check if the pool is empty.
     * @return true, if the pool is empty, false otherwise
     */
    bool empty(void) const
    {
        return _pool.empty();
    }

    /**
     * @brief singleton
     * Function returning a singleton for de ObjectPoolImpl.
     * This is the preferred method for pooling objects.
     * @return
     */
    static ObjectPoolImpl<T, Size> *singleton()
    {
        static ObjectPoolImpl<T, Size> op;
        return &op;
    }

protected:
    /**
     * T must be derived from C
     */

    /**
     * @brief createRawInner
     * Tries to return an object from the pool. If it is not possible
     * then creates a new instance.
     * You'll probably want to call the create() functions instead of this one.
     * The pointer returned with this function must be wrapped by a
     * smart pointer like Ptr<> or Cow<>.
     * @return A raw pointer to the created or recycled object. It is your
     * responsability to manually recicle this object using the recycle(obj)
     * function (Use Ptr<> instead!).
     */
    template<typename C = T>
    T *createRawInner()
    {
        T *obj = nullptr;
        if (!_pool.pop(obj))
        {
            // The pool is empty so a normal creation is needed
            obj = PrivateFactory::instanceCreator<T, C>(0);
        }
        else
        {
        }
        return obj;
    }

    /**
     * @brief createRawInner
     * Pool creation function.
     * Returns a QSharedPointer<C> pointing to the just created object.
     * The class of the object does not implements reference counting.
     * You should use the QEnableSharedFromThis base class for those
     * classes definitions.
     */
    template<typename C = T>
    typename std::enable_if < !has_goap_ref_counter<C>::value
    && !has_member_QAtomicInt__ref<C>::value, DefaultSharedPointer<C >>::type
            createVersatile()
    {
        C *obj = createRawInner();
        // Automatically collects obj when references reaches 0.
        return DefaultSharedPointer<C>(obj, [ = ](C *)
        {
            if (!recycle(obj))
            {
                PrivateFactory::instanceDeleter(obj, 0);
            }
        });
    }

    ////////////////////////////////////////////////////////////////////////////
    /// CreateVersatile
    ///
    /**
     * @brief create
     * Pool creation function for objects implementing reference counting
     * based in the functions addRef(), releaseRef(), load() and suicide().
     * Returns a Ptr<C> pointing to the just created object.
     */
    template<typename C = T>
    typename std::enable_if<has_goap_ref_counter<C>::value, Ptr<SuicideToPool<C>>>::type
    createVersatile()
    {
        // Change the suicide() function.
        T *obj = createRawInner<SuicideToPool<C>>();
        auto wrp = static_cast<SuicideToPool<C>*>(obj);
        // Automatically collects obj.
        return Ptr<SuicideToPool<C>>(wrp);
    }

    /**
     * @brief create
     * Pool creation function for objects implementing reference
     * counting based in the QSharedData.
     * Returns a Ptr<C> pointing to the just created object.
     */
    template<typename C = T>
    typename std::enable_if < !has_goap_ref_counter<C>::value
    && has_member_QAtomicInt__ref<C>::value, Ptr<SuicideToPool<WrapQSharedData<C>> >>::type
            createVersatile()
    {
        // Change the suicide() function.
        T *obj = createRawInner<SuicideToPool<WrapQSharedData<C>>>();
        auto wrp = static_cast<SuicideToPool<WrapQSharedData<C>>*>(obj);
        // Automatically collects obj.
        return Ptr<SuicideToPool<WrapQSharedData<C>>>(wrp);
    }

public:
    ////////////////////////////////////////////////////////////////////////////
    /// CreateRaw
    ///
    template<typename C = T>
    typename std::enable_if<has_goap_ref_counter<C>::value, T *>::type
    createRaw()
    {
        // Change the suicide() function.
        return createRawInner<SuicideToPool<C>>();
    }

    template<typename C = T>
    typename std::enable_if < !has_goap_ref_counter<C>::value
    && has_member_QAtomicInt__ref<C>::value, T * >::type
    createRaw()
    {
        // Change the suicide() function.
        return createRawInner<SuicideToPool<WrapQSharedData<C>>>();
    }

    template<typename C = T>
    typename std::enable_if < !has_goap_ref_counter<C>::value
    && !has_member_QAtomicInt__ref<C>::value, T * >::type
    createRaw()
    {
        // Change the suicide() function.
        return createRawInner<SuicideToPool<WrapWithSharedData<C>>>();
    }
    ////////////////////////////////////////////////////////////////////////////
    /// CreateSmart
    ///
    template<typename C = T>
    typename std::enable_if < !has_goap_ref_counter<C>::value
    && !has_member_QAtomicInt__ref<C>::value, Ptr<SuicideToPool<WrapWithSharedData<C>> >>::type
            createSmart()
    {
        // Change the suicide() function.
        T *obj = createRawInner<SuicideToPool<WrapWithSharedData<C>>>();
        auto wrp = static_cast<SuicideToPool<WrapWithSharedData<C>>*>(obj);
        // Automatically collects obj.
        return Ptr<SuicideToPool<WrapWithSharedData<C>>>(wrp);
    }

    template<typename C = T>
    auto createSmart() -> typename std::enable_if <
    has_goap_ref_counter<C>::value || has_member_QAtomicInt__ref<C>::value,
                       decltype(this->createVersatile<C>()) >::type
    {
        return createVersatile<C>();
    }

    /**
     * @brief recycle
     * Manually recycle objects.
     * This should be used when the createRawInner() function was directly used for
     * object creation.
     * @param obj
     * @param bDeleteIfCantRecycle
     * @return true if recycled or deleted. false i
     */
    bool recycle(T *obj, bool bDeleteIfCantRecycle = false)
    {
        bool ret = false;

        if (_state == STATE_ON)
        {
            clearObject(obj);
            ret = _pool.push(obj);
        }
        if(!ret && bDeleteIfCantRecycle)
        {
            // The pool can't be used, then release the object
            PrivateFactory::instanceDeleter(obj, 0);
            ret = true;
        }
        if (!ret)
        {
            /// TODO: Use the logger
        }
        return ret;
    }

};

/**
 * @brief The ObjectPool class
 * This class is the frontend of the ObjectPoolImpl<> class.
 * It only adds the smart_type typedef.
 */
template <typename T, size_t Size = DEFAULT_POOL_SIZE>
class ObjectPool : public ObjectPoolImpl<T, Size>
{
    typedef ObjectPoolImpl<T, Size> parent;

    ObjectPool(const ObjectPool<T, Size> &); // Disable copy contructor
    /**
    * @brief ObjectPool
    * You must create pool instances using ObjectPoolImpl<T, Size>::singleton()
    */
    ObjectPool() : parent() {}
public:
    virtual ~ObjectPool()
    {
        parent::~ObjectPoolImpl();
    }
    /**
     * @brief smart_type
     * The type to be returned when you call createSmart() member function.
     * The most common type the ObjectPool could return is a Ptr<T> if T
     * match the requeriments for being used directly by Ptr<>. If this is not
     * the case, the ObjectPool wraps the T type with the needed complements
     * to be used for Ptr<>.
     */
    typedef decltype(static_cast<ObjectPoolImpl<T, Size>*>(nullptr)->createSmart()) smart_type;

};

}

#endif // OBJECTPOOL_H
