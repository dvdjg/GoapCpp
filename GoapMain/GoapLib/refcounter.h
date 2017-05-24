#ifndef REFCOUNTER_H
#define REFCOUNTER_H

#include <boost/intrusive_ptr.hpp>
#include <boost/atomic.hpp>
#include "hasmember.h"

#define IMPLEMENT_REFCOUNTER_FUNCTIONS(_refcount) \
    private: \
    virtual void suicide() { instanceDeleter(this); } \
    inline int load() const { return _refcount.load(std::memory_order_relaxed); } \
    inline void store(int newValue) { _refcount.store(newValue, std::memory_order_relaxed); } \
    inline int addRef() const { return ++_refcount; } \
    inline int releaseRef() const { return --_refcount; }

/*
    template<typename T> \
    friend void intrusive_ptr_add_ref(T* t); \
    template<typename T> \
    friend void intrusive_ptr_release(T* t);
*/

#define IMPLEMENT_REFCOUNTER() \
    private: \
    mutable std::atomic<int> _refcount{0}; \
    IMPLEMENT_REFCOUNTER_FUNCTIONS(_refcount)

/** @brief GOAP_DECLARE_SHARED_PARENT macro
    Use this macro to bypass the reference counter calls to a parent implementor
*/
#define IMPLEMENT_REFCOUNTER_PARENT(parent)  \
    public: \
    virtual void suicide() { parent::suicide(); } \
    inline int load() const { return parent::load(); } \
    inline void store(int count) { parent::store(count); } \
    inline int addRef() const { return parent::addRef(); } \
    inline int releaseRef() const { return parent::releaseRef(); }

#define IMPLEMENT_REFCOUNTER_DUMMY \
    public: \
    virtual void suicide() { } \
    inline int load() const { return -1; } \
    inline void store(int) {  } \
    inline int addRef() const { return -1; } \
    inline int releaseRef() const { return -1; }

namespace goap {

/**
    @brief instanceDeleter
    This function will be used to issue a deletion when T implements the
    `deleteLater()` member function (eg. QObject derived classes).
    @param obj The address of the object to delete.
*/
template <typename T>
inline typename std::enable_if <has_member_void__deleteLater<T>::value, void>::type
instanceDeleter(T* t)
{
    if (t)
        t->deleteLater();
}

#if defined(QT_CORE_LIB) || defined(Q_SET_OBJECT_NAME)
/**
    @brief instanceDeleter
    This function will be used to delete instances. If the source is polimorphic and implements QObject,
    then use the deleteLater() function. In other cases use the object `delete` keyword.
    @param obj The address of the object to delete.
*/
template <typename T>
inline typename std::enable_if < !has_member_void__deleteLater<T>::value&& std::is_polymorphic<T>::value, void >::type
instanceDeleter(T* obj)
{
    if (obj)
    {
        QObject* pQObject = dynamic_cast<QObject*>(obj);
        if (pQObject)
        {
            instanceDeleter(pQObject);
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
typename std::enable_if < !has_member_void__deleteLater<T>::value&& !std::is_polymorphic<T>::value, void >::type
instanceDeleter(T* obj)
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
instanceDeleter(T* obj)
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
inline typename std::enable_if < has_member_void__suicide<T>::value, void >::type
instanceSuicider(T* t)
{
    if (t)
        t->suicide();
}

/**
    @brief instanceSuicider
    @param t
    Helper function that deletes the pointed object when the T::suicide()
    member function if not available.
*/
template<class T>
inline typename std::enable_if < !has_member_void__suicide<T>::value, void >::type
instanceSuicider(T* t)
{
    instanceDeleter(t);
}

/*
template<typename T>
typename std::enable_if < has_ref_counter<T>::value, void>::type
intrusive_ptr_add_ref(T* t)
{
    t->addRef();
}

template<typename T>
typename std::enable_if < has_ref_counter<T>::value, void>::type
intrusive_ptr_release(T* t)
{
    if (t->releaseRef() == 0)
        instanceSuicider(t);
}
*/
}
#endif // REFCOUNTER_H
