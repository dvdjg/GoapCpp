#ifndef GOAP_IPRIORITIZED_H
#define GOAP_IPRIORITIZED_H

#include "common/irefcounter.h"
#include "goap/ipath.h"

namespace goap
{

class IPrioritized : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IPrioritized> Ptr;
    typedef explicit_ptr<const IPrioritized> CPtr;

    virtual bool isEmpty() const = 0;
    virtual std::size_t size() const = 0;
    virtual void push(IPath::CPtr path) = 0;
    virtual void pushLazy(IPath::CPtr path) = 0;
    virtual IPath::CPtr pop() = 0;
    virtual void clear() = 0;
};

}

#endif // GOAP_IPRIORITIZED_H
