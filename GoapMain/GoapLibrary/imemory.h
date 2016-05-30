#ifndef IMEMORY_H
#define IMEMORY_H

#include <memory>
#include "iroot.h"
#include "istatevalue.h"

class IMemory : public IRoot
{
public:
    virtual ptr_value at(const ptr_value &key) const = 0;
    virtual ptr_value& setAt(const ptr_value &key, const ptr_value &value) = 0;


};

#endif // IMEMORY_H
