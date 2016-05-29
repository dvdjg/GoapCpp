#include "memory.h"

Memory::Memory()
{

}


std::shared_ptr<IStateValue> Memory::at(const std::shared_ptr<IStateValue> &key) const
{
    return data.at(key);
}

std::shared_ptr<IStateValue> &Memory::setAt(const std::shared_ptr<IStateValue> &key, const std::shared_ptr<IStateValue> &value)
{
    return data[key] = value;
}
