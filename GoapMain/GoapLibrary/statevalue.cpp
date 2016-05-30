#include <algorithm>
#include "statevalue.h"
#include "basicmath.h"

StateValue::StateValue()
{
}

std::size_t StateValue::length() const
{
    return data.size();
}

float StateValue::at(float idx) const
{
    float ret = interp2hf(idx, &data[0], static_cast<int>(data.size()));
    return ret;
}

void StateValue::fromString(const std::u16string &str)
{
    data.resize(0);
    std::copy(str.begin(), str.end(), std::back_inserter(data));
}

std::u16string StateValue::toString() const
{
    std::u16string ret(data.begin(), data.end());
    return ret;
}


void StateValue::setAt(float idx, float value)
{
    size_t i = size_t(idx);
    data[i] = value;
}


std::size_t StateValue::hash() const
{
    return ::hash(&data[0], data.size());
}

