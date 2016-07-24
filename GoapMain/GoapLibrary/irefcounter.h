#ifndef IREFCOUNTER_H
#define IREFCOUNTER_H

#include "iroot.h"

namespace goap
{

/**
    @brief The IRefCounter class
    The object implementing this interface has an atomic reference counter.
    This is useful for using SmartPointers or ObjectPools.
*/
class IRefCounter : virtual public IRoot
{
public:
    /**
        @brief load
        The returned value is only usefull when there is no other threads
        using concurrently the object reference counter.
        @return Internal counter value.
    */
    virtual int load() const = 0;

    /**
        @brief ref Increments internal reference counter.
        @return Returns true if the new value is non-zero, false otherwise.
    */
    virtual bool addRef() const = 0;

    /**
        @brief deref Atomically decrements the value of this counter.
        @return  Returns true if the new value is non-zero, false otherwise.
    */
    virtual bool releaseRef() const = 0;

    /**
        @brief suicide
        External petition for deletion.
        The implementation should do something like: "delete this"
    */
    virtual void suicide() = 0;
};

}

#endif // IREFCOUNTER_H
