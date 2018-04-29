#include "basicsinkcollection.h"
#include "basicostreamsink.h"

namespace goap
{

BasicSinkCollection::BasicSinkCollection()
{
}

BasicSinkCollection::BasicSinkCollection(const std::string &name, IBasicSink::Ptr sink)
{
    addSink(name, sink);
}

BasicSinkCollection::BasicSinkCollection(const std::string &name, std::ostream &o)
{
    addSink(name, o);
}

void BasicSinkCollection::addSink(const std::string &name, IBasicSink::Ptr sink)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sinks[name] = sink;
}

void BasicSinkCollection::addSink(const std::string &name, std::ostream &o)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sinks[name] = IBasicSink::Ptr(new BasicOstreamSink(o));
}

void BasicSinkCollection::removeSink(const std::string &name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _sinks.erase(name);
}

void BasicSinkCollection::write(const char *begin, const char *end)
{
    std::lock_guard<std::mutex> lock(_mutex);
    for (const auto &sink : _sinks) {
        sink.second->write(begin, end);
    }
}

void BasicSinkCollection::flush()
{
    std::lock_guard<std::mutex> lock(_mutex);
    for (const auto &sink : _sinks) {
        sink.second->flush();
    }
}

void BasicSinkCollection::eof() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    for (const auto &sink : _sinks) {
        sink.second->eof();
    }
}

}
