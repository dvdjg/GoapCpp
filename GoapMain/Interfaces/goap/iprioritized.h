#ifndef GOAP_IPRIORITIZED_H
#define GOAP_IPRIORITIZED_H

#include "common/irefcounter.h"
#include "goap/istringvalue.h"
#include "goap/ipath.h"

#define STR_GOAP_PRIORITIZED_QUEUE "queue"
#define STR_GOAP_PRIORITIZED_STACK "stack"

namespace goap
{

class IPrioritized : public virtual IStringPrintable
{
public:
    typedef explicit_ptr<IPrioritized> Ptr;
    typedef explicit_ptr<const IPrioritized> CPtr;

    virtual bool empty() = 0; // No const
    virtual std::int64_t size() const = 0;
    virtual void push(IPath::Ptr path) = 0;
    virtual void pushLazy(IPath::Ptr path) = 0;
    virtual IPath::Ptr pop() = 0;
    virtual void clear() = 0;
};

}

#endif // GOAP_IPRIORITIZED_H
