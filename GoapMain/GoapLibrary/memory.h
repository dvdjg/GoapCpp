#ifndef MEMORY_H
#define MEMORY_H
#include <memory>
#include <unordered_map>
#include "imemory.h"

class Memory : public IMemory, public std::enable_shared_from_this<Memory>
{
protected:
    std::unordered_map<std::shared_ptr<IStateValue>, std::shared_ptr<IStateValue>> data;
public:
    Memory();

    // IMemory interface
public:
    std::shared_ptr<IStateValue> at(const std::shared_ptr<IStateValue> & key) const override;
    std::shared_ptr<IStateValue> &setAt(const std::shared_ptr<IStateValue> &key, const std::shared_ptr<IStateValue> &value) override;
};

#endif // MEMORY_H
