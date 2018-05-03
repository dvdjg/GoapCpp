#ifndef SCOPETIME_H
#define SCOPETIME_H

#include <chrono>
#include "goap/iscopetimer.h"

namespace goap
{

class ScopeTime : public IScopeTime
{
    char _szMessage[128];
    bool _bMessageOnDelete;
    typedef std::chrono::steady_clock::time_point time_point;
    time_point _start;
public:
    typedef std::ratio<3600, 1> hour;

    ScopeTime(const char *szMessage= "", pfn_time pfnTime = nullptr, bool bMessageOnDelete = true) : _bMessageOnDelete(bMessageOnDelete), _pfnTime(pfnTime)
    {
        setMessage(szMessage);
        _start = std::chrono::steady_clock::now();
    }

    void setMessage(const char *szMessage)
    {
        *_szMessage = 0;
        if(szMessage != nullptr) {
            strncpy(_szMessage, szMessage, sizeof(_szMessage));
        }
    }

    void showSpanTime() override
    {
        if(_pfnTime == nullptr) {
            return;
        }
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
        if(_bMessageOnDelete) {
            showSpanTime();
        }
    }
    void setMessageOnDelete(bool bMessageOnDelete)
    {
        _bMessageOnDelete = bMessageOnDelete;
    }
    void setPfnTime(const pfn_time &pfnTime)
    {
        _pfnTime = pfnTime;
    }
protected:
    pfn_time _pfnTime;
};





}

#endif // SCOPETIME_H
