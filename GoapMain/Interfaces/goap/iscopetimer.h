#ifndef ISCOPETIMER_H
#define ISCOPETIMER_H

#include "common/iroot.h"

namespace goap
{

class IScopeTime : public IRoot
{
public:
    typedef void (*pfn_time)(const char *szMessage, double time, const char *szUnits);
    virtual void showSpanTime() = 0;
};

}

#endif // ISCOPETIMER_H
