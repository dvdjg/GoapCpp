#ifndef BASICSINKCOLLECTION_H
#define BASICSINKCOLLECTION_H

#include <mutex>
#include <map>
#include "goap/ibasicsink.h"

namespace goap
{

class BasicSinkCollection : public IBasicSinkCollection
{
    mutable std::mutex _mutex;
    std::map<std::string, IBasicSink::Ptr> _sinks;
public:
    typedef std::shared_ptr<BasicSinkCollection> Ptr;

    BasicSinkCollection();
    BasicSinkCollection(const std::string& name, IBasicSink::Ptr sink);
    BasicSinkCollection(const std::string& name, std::ostream &o);

    void addSink(const std::string& name, IBasicSink::Ptr sink) override;
    void addSink(const std::string& name, std::ostream &o) override;
    void removeSink(const std::string& name) override;

    // IBasicSink interface
public:
    void write(const char *begin, const char *end) override;
    void flush() override;
    void eof() const override;
};

}

#endif // BASICSINKCOLLECTION_H