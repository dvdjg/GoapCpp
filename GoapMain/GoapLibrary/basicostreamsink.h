#ifndef BASICOSTREAMSINK_H
#define BASICOSTREAMSINK_H

#include <iostream>
#include "goap/ibasicsink.h"

namespace goap
{

class BasicOstreamSink : public IBasicSink
{
    std::ostream &_ostream;
public:
    typedef std::shared_ptr<BasicOstreamSink> Ptr;

    BasicOstreamSink(const BasicOstreamSink& o) : _ostream(o._ostream)
    {
    }
    BasicOstreamSink(std::basic_ostream< char, std::char_traits< char >> &ostream = std::cerr) : _ostream(ostream)
    {
    }
    virtual ~BasicOstreamSink()
    {
        flush();
    }

    // IBasicSink interface
    void write(const char *begin, const char *end) override
    {
        _ostream.write(begin, end - begin);
    }
    void flush() override
    {
        _ostream.flush();
    }
    void eof() const override
    {
        _ostream.eof();
    }
};

}

#endif // BASICOSTREAMSINK_H
