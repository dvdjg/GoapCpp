#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#include <unordered_map>
#include "imemory.h"

class Memory : public IMemory, public std::enable_shared_from_this<Memory>
{
protected:
    std::unordered_map<ptr_value, ptr_value> data;
public:
    Memory();

    // IMemory interface
public:
    ptr_value at(const ptr_value & key) const override;
    ptr_value &setAt(const ptr_value &key, const ptr_value &value) override;
};

#endif // MEMORY_H
