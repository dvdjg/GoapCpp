#include "memory.h"

Memory::Memory()
{

}


ptr_value Memory::at(const ptr_value &key) const
{
    return data.at(key);
}

ptr_value &Memory::setAt(const ptr_value &key, const ptr_value &value)
{
    return data[key] = value;
}
