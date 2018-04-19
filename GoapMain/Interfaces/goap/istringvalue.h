#ifndef ISTRINGVALUE_H
#define ISTRINGVALUE_H

#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{
class IStringValue : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IStringValue> Ptr;
    typedef explicit_ptr<const IStringValue> CPtr;
    virtual void fromString(const std::string &str) = 0;
    virtual std::string toString() const = 0;
};

}

#endif // ISTRINGVALUE_H
