#ifndef GOAP_PRIORITIZED_H
#define GOAP_PRIORITIZED_H

#include "goap/iprioritized.h"

namespace goap
{

class PrioritizedQueue : public virtual IPrioritized
{
public:
    typedef explicit_ptr<PrioritizedQueue> Ptr;
    typedef explicit_ptr<const PrioritizedQueue> CPtr;

    virtual bool isEmpty() const = 0;
    virtual std::size_t size() const = 0;
    virtual void push(IPath::CPtr path) = 0;
    virtual void pushLazy(IPath::CPtr path) = 0;
    virtual IPath::CPtr pop() = 0;
    virtual void clear() = 0;
};

}

#endif // GOAP_PRIORITIZED_H
