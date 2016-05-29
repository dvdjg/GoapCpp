#ifndef IMEMORY_H
#define IMEMORY_H

#include <memory>
#include "iroot.h"
#include "istatevalue.h"

class IMemory : public IRoot
{
public:
    virtual std::shared_ptr<IStateValue> at(const std::shared_ptr<IStateValue> &key) const = 0;
    virtual std::shared_ptr<IStateValue>& setAt(const std::shared_ptr<IStateValue> &key, const std::shared_ptr<IStateValue> &value) = 0;
};

#endif // IMEMORY_H
