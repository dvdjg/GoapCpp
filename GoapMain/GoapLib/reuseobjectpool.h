#ifndef REUSEOBJECTPOOL_H
#define REUSEOBJECTPOOL_H

#include <mutex>
#include <queue>
#include <memory>
#include <boost/intrusive_ptr.hpp>
#include "instancedeleter.h"

namespace goap
{

template<typename T>
class ReuseObjectPool
{
public:
    typedef std::queue<T *> pool_type;
    typedef typename pool_type::size_type size_type;
    typedef std::function <T*()> function_type;
    typedef std::function <void(T *)> delete_function_type;

    template<typename F, typename D>
    ReuseObjectPool(F &&func, D &&delete_func, size_type maxSize = 512) : _maxSize(maxSize), _func(std::forward<F>(func)), _delete_func(std::forward<D>(delete_func)) {}
    virtual ~ReuseObjectPool()
    {
        clear();
    }
    static ReuseObjectPool<T> *singleton()
    {
        static ReuseObjectPool<T> pool{[]()
        {
            return new T;
        }, [](T * t)
        {
            instanceDeleter(t);
        }};
        return &pool;
    }
    void clear()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        while (!_pool.empty())
        {
            T *ret = _pool.front();
            _pool.pop();
            _delete_func(ret);
        }
    }
    size_type maxSize() const
    {
        return _maxSize;
    }

    void setMaxSize(size_type maxSize)
    {
        _maxSize = maxSize;
    }

    T *create()
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!_pool.empty())
            {
                T *ret = _pool.front();
                _pool.pop();
                return ret;
            }
        }
        return _func();
    }

    void recycle(T *t)
    {
        clearObject(t);
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_pool.size() < _maxSize)
            {
                _pool.push(t);
                return;
            }
        }
        _delete_func(t);
    }

protected:
    /**
        @brief clearObject
        Helper object for calling clear() method if available.
        @param obj
    */
    template<typename C = T>
    static typename std::enable_if<has_member_void__clear<C>::value, void>::type
    clearObject(C *obj)
    {
        return obj->clear();
    }

    template<typename C = T>
    static typename std::enable_if < !has_member_void__clear<C>::value, void >::type
    clearObject(C *)
    {} // No clear object contents

private:
    std::mutex _mutex;
    pool_type _pool;
    size_type _maxSize;
    function_type _func;
    delete_function_type _delete_func;
};

template<typename P>
class RecyclableWrapper : public P
{
public:
    typedef ReuseObjectPool<RecyclableWrapper<P>> pool_type;
    typedef typename std::conditional<has_intrusive_ptr<RecyclableWrapper<P>>::value, boost::intrusive_ptr<RecyclableWrapper<P>>, std::shared_ptr<RecyclableWrapper<P>>>::type smart_pointer;
    RecyclableWrapper() : P() {}
    virtual void suicide()
    {
        pool_type::singleton()->recycle(this);
    }
    static RecyclableWrapper<P> *createFromPoolRaw()
    {
        return pool_type::singleton()->create();
    }
    template<typename R = RecyclableWrapper<P>>
    static typename std::enable_if <has_intrusive_ptr<R>::value, boost::intrusive_ptr<R>>::type
            createFromPool()
    {
        return createFromPoolRaw();
    }
    template<typename R = RecyclableWrapper<P>>
    static typename std::enable_if < !has_intrusive_ptr<R>::value, std::shared_ptr<R >>::type
            createFromPool()
    {
        return std::shared_ptr<R>(createFromPoolRaw(), [ ](R * p)
        {
            p->suicide();
        });
    }
};
}

#endif // REUSEOBJECTPOOL_H
