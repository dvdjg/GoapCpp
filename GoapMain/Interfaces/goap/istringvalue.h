#ifndef GOAP_ISTRINGVALUE_H
#define GOAP_ISTRINGVALUE_H

#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{
class IStringPrintable : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IStringPrintable> Ptr;
    typedef explicit_ptr<const IStringPrintable> CPtr;
    virtual std::string toDebugString() const = 0;
    virtual std::string toString() const = 0;
};

class IStringValue : public virtual  IStringPrintable
{
public:
    typedef explicit_ptr<IStringValue> Ptr;
    typedef explicit_ptr<const IStringValue> CPtr;
    virtual void fromString(const std::string &str) = 0;
    virtual void assign(const std::string &other) = 0;
    virtual bool equals(const std::string &other) const = 0;
};

}

#endif // GOAP_ISTRINGVALUE_H
