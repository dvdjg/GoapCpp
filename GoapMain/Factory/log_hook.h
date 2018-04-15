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

class LOG_CONF {
    static ostream& getOstr() {
        return cerr;
    }
public:
    typedef ostream& (*pOstrType)();
    pOstrType afnOstr[4] {getOstr, nullptr, nullptr, nullptr};
    bool headers = true;
    bool datetime = true;
    loglevel level;

    LOG_CONF(loglevel lvl = WARN) : level(lvl) {}
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


class LOG {
    LOG_CONF &_logConf;
    loglevel msglevel = DEBUG;
public:
    LOG(loglevel type = DEBUG, LOG_CONF &logConf = LOG_CONF::singleton()) :_logConf(logConf), msglevel(type) {
        if(_logConf.datetime) {
            char result[32];
            *this << nowTime(result) << " ";
        }
        if(_logConf.headers) {
            *this << "[" << getLabel(type) << "] ";
        }
    }
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
        if(msglevel >= _logConf.level) {
            _logConf << msg;
        }
        return *this;
    }
private:
    inline const char* getLabel(loglevel type) {
        const char* label;
        switch(type) {
            case DEBUG: label = "DEBUG"; break;
            case INFO:  label = "INFO "; break;
            case WARN:  label = "WARN "; break;
            case ERROR: label = "ERROR"; break;
        }
        return label;
    }
};

}

#endif  /* LOG_HOOK_H */
