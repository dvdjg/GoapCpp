#ifndef SCOPETIME_H
#define SCOPETIME_H

#include <chrono>
#include "goap/iscopetimer.h"

namespace goap
{

class ScopeTime : public IScopeTime
{
    const char *_szMessage;
    bool _bOutOfScope;
    typedef std::chrono::steady_clock::time_point time_point;
    time_point _start;
public:
    typedef std::ratio<3600, 1> hour;

    ScopeTime(const char *szMessage, pfn_time pfnTime = nullptr, bool bOutOfScope = true) : _szMessage(szMessage), _bOutOfScope(bOutOfScope), _pfnTime(pfnTime)
    {
        _start = std::chrono::steady_clock::now();
    }
    void showSpanTime() override
    {
        double dblTime = 0;
        const char *szUnits = "";
        time_point end = std::chrono::steady_clock::now();
        auto diff = end - _start;
        auto count = diff.count();
        if(count < 10000LL) {
            dblTime = std::chrono::duration <double, std::nano> (diff).count();
            szUnits = "ns";
        } else if(count < 10000000LL) {
            dblTime = std::chrono::duration <double, std::micro> (diff).count();
            szUnits = "us";
        } else if(count < 10000000000LL) {
            dblTime = std::chrono::duration <double, std::milli> (diff).count();
            szUnits = "ms";
        } else if(count < 36000000000000LL){
            dblTime = std::chrono::duration <double> (diff).count();
            szUnits = "s";
        }else {
            dblTime = std::chrono::duration <double, hour> (diff).count();
            szUnits = "h";
        }

        _pfnTime(_szMessage, dblTime, szUnits);
    }
    virtual ~ScopeTime()
    {
        if(_bOutOfScope) {
            showSpanTime();
        }
    }
protected:
    pfn_time _pfnTime;
};

}

#endif // SCOPETIME_H
