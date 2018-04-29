#ifndef IBASICSINK_H
#define IBASICSINK_H

#include <memory>
#include <ostream>
#include <string>
#include "common/iroot.h"

namespace goap
{

class IBasicSink : public IRoot
{
public:
    typedef std::shared_ptr<IBasicSink> Ptr;

    virtual void write(const char *begin, const char *end) = 0;
    virtual void flush(void) = 0;
    virtual void eof(void) const = 0;
};

class IBasicSinkCollection : public IBasicSink
{
public:
    typedef std::shared_ptr<IBasicSinkCollection> Ptr;

    virtual void addSink(const std::string& name, IBasicSink::Ptr sink) = 0;
    virtual void addSink(const std::string& name, std::ostream &o) = 0;
    virtual void removeSink(const std::string& name) = 0;
};
}

#endif // IBASICSINK_H
