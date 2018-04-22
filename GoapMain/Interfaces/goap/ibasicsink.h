#ifndef IBASICSINK_H
#define IBASICSINK_H

#include <memory>
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

}

#endif // IBASICSINK_H
