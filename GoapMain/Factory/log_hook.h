#ifndef LOG_HOOK_H
#define LOG_HOOK_H

#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

#include <Windows.h>
#ifdef ERROR
#undef ERROR
#endif
namespace goap
{
inline const char* nowTime(char result[])
{
    // char result[100];
    const int MAX_LEN = 32;
    char buffer[MAX_LEN];
    if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, "HH':'mm':'ss", buffer, MAX_LEN) == 0)
        return "Error in NowTime()";

    result[0] = 0;
    static DWORD first = GetTickCount();
    sprintf_s(result, MAX_LEN, "%s.%06ld", buffer, (long)(GetTickCount() - first));
    return result;
}
}
#else

#include <sys/time.h>
namespace goap
{
inline const char* nowTime(char result[])
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    char buffer[32];
    tm r;
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&tv.tv_sec, &r));
    result[0] = 0;
    sprintf(result, "%s.%06ld", buffer, (long)tv.tv_usec);
    return result;
}
}
#endif //WIN32


namespace goap
{
using namespace std;

enum loglevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

struct structlog {
    bool headers = false;
    loglevel level = WARN;
};

extern structlog LOGCFG;


class LOG_CONF;
inline LOG_CONF& logConfSingleton();

class LOG {
    LOG_CONF &_logConf;
    loglevel _debugLevel = DEBUG;
public:

    LOG(loglevel type = DEBUG, LOG_CONF &logConf = logConfSingleton());
    ~LOG() {
        *this << '\n';
    }
//    inline LOG &endl(LOG &l) {
//        if(msglevel >= _logConf.level) {
//            _logConf << endl;
//        }
//         return *this;
//    }

    template<class T>
    LOG &operator<<(const T &msg) {
        if(_debugLevel >= _logConf.level) {
            _logConf << msg;
        }
        return *this;
    }
public:
    inline static const char* getLabel(loglevel type) {
        const char* label;
        switch(type) {
            case DEBUG: label = "DEBUG"; break;
            case INFO:  label = "INFO "; break;
            case WARN:  label = "WARN "; break;
            case ERROR: label = "ERROR"; break;
        }
        return label;
    }

    inline loglevel getDebugLevel() const
    {
        return _debugLevel;
    }

    inline void setDebugLevel(const loglevel &debugLevel)
    {
        _debugLevel = debugLevel;
    }
};


class LOG_CONF {
    static ostream& getOstr() {
        return cerr;
    }
public:
    static void defaultLogHead(LOG &log) {
        char result[32];
        log << nowTime(result) << " ";
        log << "[" << LOG::getLabel(log.getDebugLevel()) << "] ";
    }

    typedef void (*pLogHeadType)(LOG &log);
    loglevel level;
    pLogHeadType pLogHead;
    typedef ostream& (*pOstrType)();
    pOstrType afnOstr[4] {getOstr, nullptr, nullptr, nullptr};

    LOG_CONF(loglevel lvl = WARN, pLogHeadType pLogHead_ = &defaultLogHead) : level(lvl), pLogHead(pLogHead_) {}
    ~LOG_CONF() {}

    static LOG_CONF& singleton() {
        static LOG_CONF logConf;
        return logConf;
    }

    template<class T>
    LOG_CONF &operator<<(const T &msg) {
        for(pOstrType fnOstr : afnOstr) {
            if(fnOstr != nullptr) {
                fnOstr() << msg;
            }
        }
        return *this;
    }
//    inline LOG_CONF &endl(LOG_CONF &l) {
//        for(pOstrType fnOstr : afnOstr) {
//            if(fnOstr != nullptr) {
//                fnOstr() << std::endl;
//            }
//        }
//        return *this;
//    }

};

inline LOG_CONF& logConfSingleton()
{
    return LOG_CONF::singleton();
}


inline LOG::LOG(loglevel type, LOG_CONF &logConf) :_logConf(logConf), _debugLevel(type) {
    _logConf.pLogHead(*this);
}
}

#endif  /* LOG_HOOK_H */
