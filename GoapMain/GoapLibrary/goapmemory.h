#ifndef GOAPMEMORY_H
#define GOAPMEMORY_H

#include "refcounter.h"
#include "goap/imemory.h"

namespace goap
{

class Memory : public virtual IMemory
{
    IMPLEMENT_REFCOUNTER()

protected:

public:
    Memory();

    // IMemory interface
public:
//    ptr_value at(const ptr_value & key) const override;
//    ptr_value &setAt(const ptr_value &key, const ptr_value &value) override;
};

}

#endif // MEMORY_H