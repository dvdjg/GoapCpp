#ifndef GOAP_IMEMORYVALUE_H
#define GOAP_IMEMORYVALUE_H

#include "common/irefcounter.h"

namespace goap
{

enum MemoryValueType
{
    MVAL_NULL,
    MVAL_NUMBER = 0x10,
    //MVAL_FLOAT64 = 0x10,
    //MVAL_FLOAT32,
    //MVAL_FLOAT16,
    MVAL_STRING = 0x40,
    MVAL_PATH
};

class IMemoryValue : public IRefCounter
{
public:
    virtual MemoryValueType getType() const = 0;
    virtual float getNumber() const = 0;
};

}

#endif // GOAP_IMEMORYVALUE_H
