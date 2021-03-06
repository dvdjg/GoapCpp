#ifndef GOAP_SCOPETIMEOSTREAM_H
#define GOAP_SCOPETIMEOSTREAM_H

#include <iostream>
#include <string>
#include "scopetime.h"

namespace goap
{

template<std::ostream & os = std::cerr>
class ScopeTimeOstream : public ScopeTime
{
public:

    static void fnOstr(const char *szMessage, double time, const char *szUnits) {
        os << szMessage << time << ' ' << szUnits << std::endl;
    }
    ScopeTimeOstream(const char *szMessage = nullptr, bool bOutOfScope = true) : ScopeTime(szMessage, &fnOstr, bOutOfScope)
    {
    }
    ScopeTimeOstream(const std::string &strMessage, bool bOutOfScope = true) : ScopeTime(strMessage.c_str(), &fnOstr, bOutOfScope)
    {
    }
};

}
#endif // GOAP_SCOPETIMEOSTREAM_H
