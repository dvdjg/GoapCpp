#ifndef LOG_HOOK_H
#define LOG_HOOK_H

#include <iostream>

namespace goap
{
using namespace std;

class LOG;

#ifdef ERROR
#undef ERROR
#endif

enum loglevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

inline const char* getLogLevelLabel(loglevel type) {
    const char* label;
    switch(type) {
        case DEBUG: label = "DEBUG"; break;
        case INFO:  label = "INFO "; break;
        case WARN:  label = "WARN "; break;
        case ERROR: label = "ERROR"; break;
        default:    label = "INVALID";
    }
    return label;
}

struct structlog {
    bool headers = false;
    loglevel level = WARN;
};

extern structlog LOGCFG;

/**
 * @brief Configurations for LOGs
 */
class LOG_CONF
{
    loglevel _level;
    typedef void (*pFnLogHeadType)(LOG &log);
    pFnLogHeadType _pFnLogHead;

    static ostream& getOstr() {
        return cerr;
    }

public:

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

    friend LOG_CONF &endl(LOG_CONF &l);
    friend LOG_CONF &flush(LOG_CONF &l);
    static void defaultLogHead(LOG &log);

    typedef ostream& (*fnOstrType)();
    void setOstrFunciton(fnOstrType fn) {
        _afnOstr[0] = fn;
    }
    LOG_CONF(loglevel lvl = WARN, pFnLogHeadType pLogHead_ = &defaultLogHead) : _level(lvl), _pFnLogHead(pLogHead_) {}
    ~LOG_CONF() {}

    static LOG_CONF& singleton() {
        static LOG_CONF logConf;
        return logConf;
    }

    template<class T>
    LOG_CONF &operator<<(const T &msg) {
        for(fnOstrType fnOstr : _afnOstr) {
            if(fnOstr != nullptr) {
                fnOstr() << msg;
            }
        }
        return *this;
    }

    LOG_CONF& operator<<(LOG_CONF& (*pfn)(LOG_CONF&))
    {	// call basic manipulator
        return (*pfn)(*this);
    }

private:
    fnOstrType _afnOstr[1] {getOstr};
};

class LOG_CONF;
inline LOG_CONF& logConfSingleton();

class LOG {
    LOG_CONF &_logConf;
    loglevel _debugLevel = DEBUG;
public:

    friend LOG &endl(LOG &l);
    friend LOG &flush(LOG &l);
    LOG(loglevel type = DEBUG, LOG_CONF &logConf = logConfSingleton());

public:


    inline loglevel getDebugLevel() const
    {
        return _debugLevel;
    }

    inline void setDebugLevel(const loglevel &debugLevel)
    {
        _debugLevel = debugLevel;
    }

    LOG_CONF &getLogConf() const
    {
        return _logConf;
    }
    template<class T>
    LOG &operator<<(const T &msg) {
        if(_debugLevel >= _logConf.getLevel()) {
            _logConf << msg;
        }
        return *this;
    }
    LOG& operator<<(LOG& (*pfn)(LOG&))
    {	// call basic manipulator
        return (*pfn)(*this);
    }

    ~LOG();
};

inline void LOG_CONF::defaultLogHead(LOG &log) {
    log << "[" << getLogLevelLabel(log.getDebugLevel()) << "] ";
}

inline LOG_CONF& logConfSingleton()
{
    return LOG_CONF::singleton();
}


inline LOG::LOG(loglevel type, LOG_CONF &logConf) :_logConf(logConf), _debugLevel(type) {
    _logConf.getFnLogHead()(*this);
}


inline LOG_CONF &endl(LOG_CONF &l) {
    for(LOG_CONF::fnOstrType fnOstr : l._afnOstr) {
        if(fnOstr != nullptr) {
            fnOstr() << std::endl;
        }
    }
    return l;
}

inline LOG_CONF &flush(LOG_CONF &l) {
    for(LOG_CONF::fnOstrType fnOstr : l._afnOstr) {
        if(fnOstr != nullptr) {
            fnOstr() << std::flush;
        }
    }
    return l;
}

inline LOG &endl(LOG &l) {
    if(l.getDebugLevel() >= l.getLogConf().getLevel()) {
        l.getLogConf() << endl;
    }
    return l;
}



inline LOG &flush(LOG &l) {
    if(l.getDebugLevel() >= l.getLogConf().getLevel()) {
        l.getLogConf() << flush;
    }
    return l;
}



inline LOG::~LOG() {
    *this << endl;
}

}

#endif  /* LOG_HOOK_H */
