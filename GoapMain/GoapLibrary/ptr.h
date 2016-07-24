#ifndef PTR_H
#define PTR_H

#if defined(QT_CORE_LIB)
#include <QObject>
#endif
#include <functional>
#include "ptr_forward.h"

namespace goap
{

#ifndef GOAP_COMMA
#define GOAP_COMMA ,
#endif

/** @brief HAS_MEMBER(member_type, member_call, templ_postfix)
    Check for the existence of a member callable with the `member_call` signature
    returning a type converitible to `member_type`.
    The `member_call` works even for pure virtual function invocations.
    @sa http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
*/
#define HAS_MEMBER(member_type, member_call, templ_postfix) \
template <typename T> \
class has_member_##templ_postfix \
{ \
    typedef char one; \
    typedef long two; \
\
    template <typename C> static two test(...); \
    template <typename C> static one test( typename std::enable_if< \
        std::is_convertible<decltype( \
        static_cast<C*>(nullptr)->member_call), member_type>::value>::type * \
    ); \
public: \
    enum { evalue = sizeof(test<T>(0)) == sizeof(char) }; \
    static bool const value = sizeof(test<T>(0)) == sizeof(char); \
}

template <typename T> static inline T *
dGetPtrHelper(T *ptr)
{
    return ptr;
}
template <typename Wrapper> static inline typename Wrapper::pointer
dGetPtrHelper(const Wrapper &p)
{
    return const_cast<typename Wrapper::pointer>(p.data());
}

// Helper testers to determine if a class implements a functionality.
HAS_MEMBER(bool, releaseRef(), bool__releaseRef);
HAS_MEMBER(bool, addRef(), bool__addRef);
HAS_MEMBER(int, load(), int__load);
HAS_MEMBER(void, suicide(), void__suicide);
HAS_MEMBER(void *, clone(), voidp__clone);
HAS_MEMBER(void *, get(), voidp__get);
HAS_MEMBER(void *, data(), voidp__data);
HAS_MEMBER(void, reset(), void__reset);
HAS_MEMBER(const void *, constData(), voidp__constData);
HAS_MEMBER(QAtomicInt, ref, QAtomicInt__ref);
HAS_MEMBER(void, deleteLater(), void__deleteLater);

/**
    PrivateFactory Internal hooks for object creation and deletion.
*/
namespace PrivateFactory
{
/**
    @brief instanceCreator
    This function will be used to create instances using the object
    T::instance<C>() static function if available.
    This function can be overloaded by the user for his own creation procedure.
    @return The address of the created object.
*/
template <typename T, typename C = T>
inline auto instanceCreator(int) -> decltype((&T::instance)())
{
    return T::instance();
}

/**
    @brief instanceCreator
    This function will be used to create instances by excplicity creating
    the object when no T::instance<C>() static function is available.
    @note Please, consider using GOAP_DECLARE_SHARED when declaring the main
    class implementation or derive it from GoapSharedData, QSharedData,
    or QEnableSharedFromThis.
    @sa http://doc.qt.io/qt-5/qenablesharedfromthis.html
    @sa http://doc.qt.io/qt-5.6/qshareddata.html
    @return The address of the created object.
*/
template <typename T, typename C = T>
inline C * instanceCreator(long)
{
    return new C;
}

//template <typename T, typename C = T>
//inline typename std::enable_if<!std::is_default_constructible<C>::value, C *>::type instanceCreator(long)
//{
//    static_assert(false, "You are trying to create a class that can't be constructed. Please check that all virtual methods are implemented. Have you modified an interface?");
//    return new C;
//}

// Discrimination using C++11 "trailing return type syntax" ->

/**
    @brief instanceDeleter
    This function will be used to delete instances using the object
    T::deleter() static function if available.
    @param obj The address of the object to delete.
*/
template <typename T>
inline auto instanceDeleter(T *obj, int) -> decltype(T::deleter(obj))
{
    T::deleter(obj);
}

/**
    @brief instanceDeleter
    This function will be used to issue a deletion when T implements the
    `deleteLater()` member function (eg. QObject derived classes).
    @param obj The address of the object to delete.
*/
template <typename T>
inline typename std::enable_if <has_member_void__deleteLater<T>::value, void>::type
instanceDeleter(T *obj, long)
{
    obj->deleteLater();
}

#if defined(QT_CORE_LIB) || defined(Q_SET_OBJECT_NAME)
/**
    @brief instanceDeleter
    This function will be used to delete instances. If the source is polimorphic and implements QObject,
    then use the deleteLater() function. In other cases use the object `delete` keyword.
    @param obj The address of the object to delete.
*/
template <typename T>
inline typename std::enable_if < !has_member_void__deleteLater<T>::value && std::is_polymorphic<T>::value, void >::type
instanceDeleter(T *obj, long)
{
    if (obj)
    {
        QObject *pQObject = dynamic_cast<QObject *>(obj);
        if (pQObject)
        {
            instanceDeleter(pQObject,0);
            return;
        }
    }
    delete obj;
}

/**
    @brief instanceDeleter
    This function will be used to delete instances using the object `delete` keyword.
    @param obj The address of the object to delete.
*/
template <typename T>
inline
typename std::enable_if < !has_member_void__deleteLater<T>::value && !std::is_polymorphic<T>::value, void >::type
instanceDeleter(T *obj, long)
{
    delete obj;
}
#else
/**
    @brief instanceDeleter
    This function will be used to delete instances using the object `delete` keyword.
    @param obj The address of the object to delete.
*/
template <typename T>
inline
typename std::enable_if < !has_member_void__deleteLater<T>::value, void >::type
instanceDeleter(T *obj, long)
{
    delete obj;
}
#endif

/**
    @brief instanceSuicider
    @param t
    Helper function that calls the T::suicide() member function if available.
*/
template<class T>
inline auto instanceSuicider(T *t, int) -> decltype(t->suicide(), void())
{
    if (t)
    {
        t->suicide();
    }
}

/**
    @brief instanceSuicider
    @param t
    Helper function that deletes the pointed object when the T::suicide()
    member function if not available.
*/
template<class T>
inline void instanceSuicider(T *t, long)
{
    instanceDeleter(t, 0);
}

} // PrivateFactory

constexpr inline unsigned int goapHash(unsigned int key, unsigned int seed = 0) noexcept
{
    return key ^ seed;
}

constexpr inline unsigned int goapHash(unsigned long long key, unsigned int seed = 0) noexcept
{
    return unsigned int(((key >> (8 * sizeof(unsigned int) - 1)) ^ key) & (~0U)) ^ seed;
}

template <class T> inline unsigned int goapHash(const T *key, unsigned int seed = 0) noexcept
{
    return goapHash(reinterpret_cast<uintptr_t>(key), seed);
}

/**
    @brief The PtrDeleter class
    Helper class for object deletion.
    When the static function deleter() is called then:
    - If T implements the funcion T::suicide() then it will be called.
    - Otherways a normal delete is used.
*/
template<class T>
class PtrDeleter
{
public:
    /// Allows the instance to be self deleted
    static void deleter(T *t)
    {
        PrivateFactory::instanceSuicider(t, 0);
    }
};

template<class T>
class PtrNoDeleter
{
public:
    static void deleter(T *) { } // Never deletes the instance
};

/**
    @brief Ptr Smart pointer for explicit shared data with custom deleter.
    Similar to QExplicitlySharedDataPointer but
    using a IRefCounter interface.

    If you need a object clone, do call detach().
    The T type must implement the load(), addRef(), and releaseRef() functions.
    The D type must implement a static deleter().
*/
template <class T, class D>
class Ptr
{
public:
    typedef T Type;
    typedef T *pointer;
    typedef D Deleter;

    static void deleter(T *t)
    {
        D::deleter(t);
    }

    inline T &operator*() const
    {
        return *d;
    }
    inline T *operator->()
    {
        return d;
    }
    inline T *operator->() const
    {
        return d;
    }
    inline T *data() const
    {
        return d;
    }
    inline const T *constData() const
    {
        return d;
    }

    inline void detach()
    {
        if (d && d->load() != 1)
        {
            detach_helper();
        }
    }

    inline void reset()
    {
        if (d && !d->releaseRef())
        {
            Ptr::deleter(d);
        }
        d = nullptr;
    }

    inline operator bool () const
    {
        return d != nullptr;
    }

    inline bool operator==(const Ptr<T, D> &other) const
    {
        return d == other.d;
    }
    inline bool operator!=(const Ptr<T, D> &other) const
    {
        return d != other.d;
    }
    inline bool operator==(const T *ptr) const
    {
        return d == ptr;
    }
    inline bool operator!=(const T *ptr) const
    {
        return d != ptr;
    }

    inline Ptr() : d(nullptr)
    {
    }
    inline ~Ptr()
    {
        if (d && !d->releaseRef())
        {
            Ptr<T, D>::deleter(d);
        }
    }

    inline Ptr(T *data) noexcept;
    inline Ptr(const Ptr<T, D> &o) : d(o.d)
    {
        if (d)
        {
            d->addRef();
        }
    }

    template<class X>
    inline Ptr(const Ptr<X, D> &o)
        : d(o.data())
    {
        if (d)
        {
            d->addRef();
        }
    }

    inline Ptr<T, D> &operator=(const Ptr<T, D> &o)
    {
        if (o.d != d)
        {
            if (o.d)
            {
                o.d->addRef();
            }
            T *old = d;
            d = o.d;
            if (old && !old->releaseRef())
            {
                Ptr::deleter(old);
            }
        }
        return *this;
    }
    inline Ptr<T, D> &operator=(T *o)
    {
        if (o != d)
        {
            if (o)
            {
                o->addRef();
            }
            T *old = d;
            d = o;
            if (old && !old->releaseRef())
            {
                Ptr::deleter(old);
            }
        }
        return *this;
    }
#ifdef Q_COMPILER_RVALUE_REFS
inline Ptr(Ptr<T, D> &&o) noexcept :
    d(o.d)
    {
        o.d = nullptr;
    }
    inline Ptr<T, D> &operator=(Ptr<T, D> &&other) noexcept
    {
        qSwap(d, other.d);
        return *this;
    }
#endif

    inline bool operator!() const
    {
        return !d;
    }

    inline void swap(Ptr<T, D> &other) noexcept
    {
        qSwap(d, other.d);
    }

protected:
    /**
        @brief clone
        When implementing a copy on write pattern using interfaces,
        it is needed a way for clone the real object. If the `clone()` function
        is provided, it will be used over a explicit `new` memory reservation.
    */
    template <class Ti = T>
    inline
    typename std::enable_if<has_member_voidp__clone<Ti>::value, Ti *>::type
    clone()
    {
        auto cloned = d->clone();
        if (!cloned)
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": Error while cloning object");
        }
        auto ret = dynamic_cast<Ti *>(cloned);
        if (!ret)
        {
            PrivateFactory::instanceDeleter(cloned, 0);
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": Error while casting cloned object");
        }
        return ret;
    }

    /**
        @brief clone
        Default clone to be used when no `clone()` function is available.
        That is the case of the constuctor copiable classes.
    */
    template <class Ti = T>
    inline
    typename std::enable_if < !has_member_voidp__clone<Ti>::value
    && std::is_copy_constructible<Ti>::value, Ti * >::type
    clone()
    {
        return new T(*d);
    }

    template <class Ti = T>
    inline
    typename std::enable_if < !has_member_voidp__clone<Ti>::value
    && !std::is_copy_constructible<Ti>::value, Ti * >::type
    clone()
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": The object can't be cloned. Please implement a clone() function or a copy constructor.");
        return nullptr;
    }
protected:
    void detach_helper();

    T *d;
};

template <class T, class D>
void Ptr<T, D>::detach_helper()
{
    auto x = clone();
    *this = x;
}

template <class T, class D>
inline Ptr<T, D>::Ptr(T *adata) noexcept :
d(adata)
{
    if (d)
    {
        d->addRef();
    }
}

template <class T, class D>
inline void qSwap(Ptr<T, D> &p1, Ptr<T, D> &p2)
{
    p1.swap(p2);
}

template <class T, class D>
inline unsigned int goapHash(const goap::Ptr<T, D> &ptr, unsigned int seed = 0) noexcept
{
    return goapHash(ptr.data(), seed);
}

template <class T, class D>
inline unsigned int qHash(const goap::Ptr<T, D> &ptr, unsigned int seed = 0) noexcept
{
    return goapHash(ptr.data(), seed);
}

///////////////////////////////////////////


/**
    @brief PtrBad Smart pointer for explicit shared data with custom deleter.
    Similar to Cow but
    using a IRefCounter interface.

    If you need a object clone, do call detach().
    However detach() is automatically called when the internal data is to be
    modified. This is a implementation of the Copy On Write (COW) pattern.
    The T type must implement the load(), addRef(), and releaseRef() functions.
    The D type must implement a static deleter().
*/

template <class T, class D>
class Cow
{
public:
    typedef T Type;
    typedef T *pointer;
    typedef D Deleter;

    static void deleter(T *t)
    {
        D::deleter(t);
    }

    inline void detach()
    {
        if (d)
        {
            auto load = d->load();
            if (load != 1)
            {
                detach_helper();
            }
        }
    }
    inline void reset()
    {
        if (d && !d->releaseRef())
        {
            Cow::deleter(d);
        }
        d = nullptr;
    }
    inline T &operator*()
    {
        detach();
        return *d;
    }
    inline const T &operator*() const
    {
        return *d;
    }
    inline T *operator->()
    {
        detach();
        return d;
    }
    inline const T *operator->() const
    {
        return d;
    }
    inline operator T *()
    {
        detach();
        return d;
    }
    inline operator const T *() const
    {
        return d;
    }
    inline T *data()
    {
        detach();
        return d;
    }
    inline const T *data() const
    {
        return d;
    }
    inline const T *constData() const
    {
        return d;
    }

    inline bool operator==(const Cow<T, D> &other) const
    {
        return d == other.d;
    }
    inline bool operator!=(const Cow<T, D> &other) const
    {
        return d != other.d;
    }

    inline Cow() : d {nullptr} { }
    inline ~Cow()
    {
        if (d && !d->releaseRef())
        {
            Cow<T, D>::deleter(d);
        }
    }

    explicit Cow(T *data) noexcept;
    inline Cow(const Cow<T, D> &o) : d(o.d)
    {
        if (d)
        {
            d->addRef();
        }
    }
    inline Cow<T, D> &operator=(const Cow<T, D> &o)
    {
        if (o.d != d)
        {
            if (o.d)
            {
                o.d->addRef();
            }
            T *old = d;
            d = o.d;
            if (old && !old->releaseRef())
            {
                Cow<T, D>::deleter(old);
            }
        }
        return *this;
    }
    inline Cow &operator=(T *o)
    {
        if (o != d)
        {
            if (o)
            {
                o->addRef();
            }
            T *old = d;
            d = o;
            if (old && !old->releaseRef())
            {
                Cow<T, D>::deleter(old);
            }
        }
        return *this;
    }
#ifdef Q_COMPILER_RVALUE_REFS
Cow(Cow &&o) noexcept :
    d(o.d)
    {
        o.d = nullptr;
    }
    inline Cow<T, D> &operator=(Cow<T, D> &&other) noexcept
    {
        std::swap(d, other.d);
        return *this;
    }
#endif

    inline bool operator!() const
    {
        return !d;
    }
    inline operator bool () const
    {
        return d != nullptr;
    }

    inline void swap(Cow<T, D> &other) noexcept
    {
        std::swap(d, other.d);
    }

protected:
    /**
        @brief clone
        When implementing a copy on write pattern using interfaces,
        it is needed a way for clone the real object. If the `clone()` function
        is provided, it will be used over a explicit `new` memory reservation.
    */
    template <class Ti = T>
    inline
    typename std::enable_if<has_member_voidp__clone<Ti>::value, Ti *>::type
    clone()
    {
        auto cloned = d->clone();
        if (!cloned)
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": Error while cloning object");
        }
        auto ret = dynamic_cast<Ti *>(cloned);
        if (!ret)
        {
            PrivateFactory::instanceDeleter(cloned, 0);
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": Error while casting cloned object");
        }
        return ret;
    }

    /**
        @brief clone
        Default clone to be used when no `clone()` function is available.
        That is the case of the constuctor copiable classes.
    */
    template <class Ti = T>
    inline
    typename std::enable_if < !has_member_voidp__clone<Ti>::value
    && std::is_copy_constructible<Ti>::value, Ti * >::type
    clone()
    {
        return new T(*d);
    }

    template <class Ti = T>
    inline
    typename std::enable_if < !has_member_voidp__clone<Ti>::value
    && !std::is_copy_constructible<Ti>::value, Ti * >::type
    clone()
    {
        throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + ": The object can't be cloned. Please implement a clone() function or a copy constructor.");
        return nullptr;
    }

private:
    void detach_helper();

    T *d;
};


template <class T, typename D>
inline Cow<T, D>::Cow(T *adata) noexcept
:
d(adata)
{
    if (d)
    {
        d->addRef();
    }
}

template <class T, typename D>
void Cow<T, D>::detach_helper()
{
    auto x = clone();
    *this = x;
}

template <class T, typename D>
inline void qSwap(Cow<T, D> &p1, Cow<T, D> &p2)
{
    p1.swap(p2);
}

template <class T, typename D>
inline unsigned int goapHash(const Cow<T, D> &ptr, unsigned int seed = 0) noexcept
{
    return goapHash(ptr.data(), seed);
}

template <class T, typename D>
inline unsigned int qHash(const Cow<T, D> &ptr, unsigned int seed = 0) noexcept
{
    return goapHash(ptr.data(), seed);
}
}

namespace std
{
template <class T, class D>
inline void swap(goap::Ptr<T, D> &p1, goap::Ptr<T, D> &p2)
{
    p1.swap(p2);
}
template <class T, typename D>
inline void swap(goap::Cow<T, D> &p1, goap::Cow<T, D> &p2)
{
    p1.swap(p2);
}
}

#endif // PTR_H
