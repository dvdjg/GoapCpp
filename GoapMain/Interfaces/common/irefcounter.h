#ifndef IREFCOUNTER_H
#define IREFCOUNTER_H

#include "iroot.h"
#include "utility_simple.h"

#define STR_GOAP_SINGLETON "Singleton"

namespace goap
{

/**
    @brief The IRefCounter class
    The object implementing this interface has an atomic reference counter.
    This is useful for using SmartPointers or ObjectPools.
*/
class IRefCounter : public IRoot
{
protected:
    /**
        @brief loadRef
        The returned value is only usefull when there is no other threads
        using concurrently the object reference counter.
        @return Internal counter value.
    */
    virtual int loadRef() const = 0;

    /**
        @brief ref Increments internal reference counter.
    */
    virtual int addRef() const = 0;

    /**
        @brief deref Atomically decrements the value of this counter.
    */
    virtual int releaseRef() const = 0;

    /**
        @brief suicide
        External petition for deletion.
        The implementation should do something like: "delete this"
    */
    virtual void suicide() = 0;

    friend void intrusive_ptr_add_ref(const IRefCounter *t);
    friend void intrusive_ptr_release(const IRefCounter *t);
};

template<typename T>
inline typename enable_if <is_convertible<T*, IRefCounter*>::value, void>::type
intrusive_ptr_add_ref(const T *t)
{
    intrusive_ptr_add_ref(static_cast<const IRefCounter*>(t));
}

inline void intrusive_ptr_add_ref(const IRefCounter *t)
{
    t->addRef();
}

template<typename T>
inline typename enable_if <is_convertible<T*, IRefCounter*>::value, void>::type
intrusive_ptr_release(const T *t)
{
    intrusive_ptr_release(static_cast<const IRefCounter*>(t));
}

inline void intrusive_ptr_release(const IRefCounter *t)
{
    if (t->releaseRef() == 0)
    {
        // Ouch! A deletion from a const pointer...
        const_cast<IRefCounter*>(t)->suicide();
    }
}
}

#endif // IREFCOUNTER_H
