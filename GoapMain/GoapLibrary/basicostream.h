#ifndef BASICOSTREAM_H
#define BASICOSTREAM_H

#include <mutex>
#include <iostream>
#include "goap/ibasicsink.h"

namespace goap
{

class BasicStreamBuffer : public std::basic_streambuf<char, std::char_traits<char>>
{
    mutable std::mutex _mutex;
public:

    BasicStreamBuffer(IBasicSink::Ptr data);
    ~BasicStreamBuffer();

    void setSink(IBasicSink::Ptr data);
protected:

    // For EOF detection
    typedef std::char_traits<char> Traits;

    // This is called when buffer becomes full. If
    // buffer is not used, then this is called every
    // time when characters are put to stream.
    int overflow(int c = Traits::eof()) override;

    // This function is called when stream is flushed,
    // for example when std::endl is put to stream.
    int sync(void) override;

private:

    // Work in buffer mode. It is also possible to work without buffer.
    static const size_t BUF_SIZE = 128;
    char _buf[BUF_SIZE];

    IBasicSink::Ptr _data;

    // In this function, the characters are parsed.
    inline void write(const char *begin, const char *end);

};

class BasicOStream : public std::basic_ostream< char, std::char_traits< char >>
{

public:

    BasicOStream(IBasicSink::Ptr data);
    BasicOStream();
private:

    BasicStreamBuffer _buf;
};

}

#endif // BASICOSTREAM_H
