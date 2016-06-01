#ifndef IREADSTATE_H
#define IREADSTATE_H

#include <memory>
#include "iroot.h"
#include "istatevalue.h"

class IState : public IRoot
{
public:
    virtual void remove(const ptr_value &key) const = 0;
    virtual ptr_value& setAt(const ptr_value &key, const ptr_value &value) = 0;
    virtual ptr_value at(const ptr_value &key) const = 0;
    virtual std::pair<ptr_value, ptr_value> at(ssize_t idx) const = 0;
    virtual size_t size() const = 0;
    virtual bool equals(const IState * other) const = 0;
    virtual float cost() const = 0;
    virtual float & setCost(float c) const = 0;
};

#endif // IREADSTATE_H
