#ifndef IREADSTATE_H
#define IREADSTATE_H

#include <memory>
#include "iroot.h"
#include "istatevalue.h"

namespace goap
{

class IState : public IRoot
{
public:
    virtual void remove(const PtrIValue &key) = 0;
    virtual void setAt(const PtrIValue &key, const PtrIValue &value) = 0;
    virtual PtrIValue at(const PtrIValue &key) const = 0;
    virtual void remove(const std::u16string &str) = 0;
    virtual void setAt(const std::u16string &str, const PtrIValue &value) = 0;
    virtual PtrIValue at(const std::u16string &str) const = 0;
    virtual std::pair<PtrIValue, PtrIValue> at(intptr_t idx) const = 0;
    virtual size_t size() const = 0;
    virtual bool equals(const IState *other) const = 0;
    virtual float cost() const = 0;
    virtual void setCost(float c) = 0;
};

}

#endif // IREADSTATE_H
