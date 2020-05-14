#include "basicostream.h"
#include "basicostreamsink.h"

namespace goap
{

BasicStreamBuffer::BasicStreamBuffer(IBasicSink::Ptr data) :
    _data(data)
{
    if (!_data) {
        throw std::runtime_error("Invalid data Sink");
    }
    setp(_buf, _buf + BUF_SIZE);
}

BasicStreamBuffer::~BasicStreamBuffer()
{
    sync();
}

void BasicStreamBuffer::setSink(IBasicSink::Ptr data)
{
    if (!data) {
        throw std::runtime_error("Invalid data Sink");
    }
    std::lock_guard<std::mutex> lock(_mutex);
    write(pbase(), pptr());
    _data->flush();

    _data = data;
    setp(_buf, _buf + BUF_SIZE);
}

int BasicStreamBuffer::overflow(int c)
{
#ifdef DEBUG
    std::cerr << "(over)" << std::endl;
#endif
    // Handle output
    std::lock_guard<std::mutex> lock(_mutex);
    write(pbase(), pptr());
    if (c != Traits::eof()) {
        char c2 = c;
        // Handle the one character that didn't fit to buffer
        write(&c2, &c2 + 1);
    } else {
        _data->eof();
    }
    // This tells that buffer is empty again
    setp(_buf, _buf + BUF_SIZE);

    return c;
}

int BasicStreamBuffer::sync()
{
    // Handle output
    std::lock_guard<std::mutex> lock(_mutex);
    write(pbase(), pptr());
    _data->flush();
    // This tells that buffer is empty again
    setp(_buf, _buf + BUF_SIZE);
    return 0;
}

void BasicStreamBuffer::write(const char *begin, const char *end)
{
#ifdef DEBUG
    std::cerr << "(putChars(" << static_cast<const void *>(begin) <<
                 "," << static_cast<const void *>(end) << "))" << endl;
#endif
    _data->write(begin, end);
}

BasicOStream::BasicOStream(IBasicSink::Ptr data) :
    std::basic_ostream< char, std::char_traits< char >>(&_buf),
    _buf(data)
{
}

BasicOStream::BasicOStream() :
    std::basic_ostream< char, std::char_traits< char >>(&_buf),
    _buf(IBasicSink::Ptr(new BasicOstreamSink(std::cerr)))
{
}

}
