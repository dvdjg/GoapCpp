#ifndef ISCOPETIMER_H
#define ISCOPETIMER_H

#include "common/iroot.h"
#include "explicit_ptr.h"

namespace goap
{

class IScopeTime : public IRoot
{
public:
    typedef explicit_ptr<IScopeTime> Ptr;
    typedef explicit_ptr<const IScopeTime> CPtr;

    typedef void (*pfn_time)(const char *szMessage, double time, const char *szUnits);
    virtual void showSpanTime() = 0;
};

}

#endif // ISCOPETIMER_H
