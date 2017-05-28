#ifndef INSTANCEDELETER_H
#define INSTANCEDELETER_H

#include "hasmember.h"

namespace goap
{

/**
    @brief instanceDeleter
    This function will be used to issue a deletion when T implements the
    `deleteLater()` member function (eg. QObject derived classes).
    @param obj The address of the object to delete.
*/
template <typename T>
inline typename std::enable_if <has_member_void__deleteLater<T>::value, void>::type
instanceDeleter(T *t)
{
    if (t)
    {
        t->deleteLater();
    }
}

#if defined(QT_CORE_LIB) || defined(Q_SET_OBJECT_NAME)
/**
    @brief instanceDeleter
    This function will be used to delete instances. If the source is polimorphic and implements QObject,
    then use the deleteLater() function. In other cases use the object `delete` keyword.
    @param obj The address of the object to delete.
*/
template <typename T>
inline typename std::enable_if < !has_member_void__deleteLater<T>::value &&std::is_polymorphic<T>::value, void >::type
instanceDeleter(T *obj)
{
    if (obj)
    {
        QObject *pQObject = dynamic_cast<QObject *>(obj);
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
typename std::enable_if < !has_member_void__deleteLater<T>::value &&!std::is_polymorphic<T>::value, void >::type
instanceDeleter(T *obj)
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
instanceDeleter(T *obj)
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
instanceSuicider(T *t)
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
inline typename std::enable_if < !has_member_void__suicide<T>::value, void >::type
instanceSuicider(T *t)
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
#endif // INSTANCEDELETER_H
