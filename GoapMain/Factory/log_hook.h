#ifndef LOG_HOOK_H
#define LOG_HOOK_H

#include <iostream>

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
        if(_debugLevel >= _logConf._level) {
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
    typedef void (*pFnLogHeadType)(LOG &log);
    pFnLogHeadType _pFnLogHead;

    static ostream& getOstr() {
        return cerr;
    }
public:
    static void defaultLogHead(LOG &log) {
        char result[32];
        //log << nowTime(result) << " ";
        log << "[" << LOG::getLabel(log.getDebugLevel()) << "] ";
    }

    loglevel _level;
    typedef ostream& (*pOstrType)();
    pOstrType afnOstr[4] {getOstr, nullptr, nullptr, nullptr};

    LOG_CONF(loglevel lvl = WARN, pFnLogHeadType pLogHead_ = &defaultLogHead) : _level(lvl), _pFnLogHead(pLogHead_) {}
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

    void setFnLogHead(pFnLogHeadType pFnLogHead)
    {
        _pFnLogHead = pFnLogHead;
    }
    pFnLogHeadType& getFnLogHead()
    {
        return _pFnLogHead;
    }

    loglevel getLevel() const
    {
        return _level;
    }

    void setLevel(const loglevel &level)
    {
        _level = level;
    }
};

inline LOG_CONF& logConfSingleton()
{
    return LOG_CONF::singleton();
}


inline LOG::LOG(loglevel type, LOG_CONF &logConf) :_logConf(logConf), _debugLevel(type) {
    _logConf.getFnLogHead()(*this);
}





}

#endif  /* LOG_HOOK_H */
