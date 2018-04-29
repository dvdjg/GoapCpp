#ifndef SCOPETIME_H
#define SCOPETIME_H

#include <string>
#include <iostream>
#include <chrono>
#include "goap/iscopetimer.h"

namespace goap
{

class ScopeTime : public IScopeTime
{
    const char *_szMessage;
    std::string _strMessage;
    std::ostream & _ostream;
    typedef std::chrono::steady_clock::time_point time_point;
    time_point _start;

public:
    ScopeTime(const std::string &strMessage = {}, std::ostream & o = std::cerr) : _szMessage(""), _strMessage(strMessage), _ostream(o)
    {
        _start = std::chrono::steady_clock::now();
    }
    ScopeTime(const char * szMessage, std::ostream & o = std::cerr) : _szMessage(szMessage), _ostream(o)
    {
        _start = std::chrono::steady_clock::now();
    }
    virtual ~ScopeTime()
    {
        time_point end = std::chrono::steady_clock::now();
        auto diff = end - _start;
        _ostream << _szMessage << _strMessage << ' ';
        auto count = diff.count();
        if(count < 10000LL) {
            _ostream << std::chrono::duration <double, std::nano> (diff).count() << " ns";
        } else if(count < 10000000LL) {
            _ostream << std::chrono::duration <double, std::micro> (diff).count() << " us";
        } else if(count < 10000000000LL) {
            _ostream << std::chrono::duration <double, std::milli> (diff).count() << " ms";
        } else {
            _ostream << std::chrono::duration <double> (diff).count() << " s";
        }
        _ostream << std::endl;
    }
};

}

#endif // SCOPETIME_H
