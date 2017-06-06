#include <stdexcept>
#include "statevalue.h"
#include "basicmath.h"

namespace goap
{

StateValue::StateValue()
{
}

StateValue::StateValue(const StateValue &other) : parent(other), data(other.data)
{
}

StateValue::StateValue(const std::u16string &str)
{
    fromString(str);
}

std::size_t StateValue::size() const
{
    return data.size();
}

void StateValue::resize(std::size_t len)
{
    data.resize(len);
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

void StateValue::interpolateFrom(const IStateValue * other)
{
    auto o = dynamic_cast<const StateValue *>(other);
    if (!o)
        throw new std::runtime_error(__func__);
    resize(o->size());
    if (size() > 0)
        interp2arrayhf(&o->data[0], o->size(), &data[0], size());
}

float StateValue::cosineDistance(const IStateValue *other) const
{
    auto o = dynamic_cast<const StateValue *>(other);
    if (!o)
        throw new std::runtime_error(__func__);
    float min = std::min(data.size(), o->data.size());

    auto dist = fnCosineDistance(data.cbegin(), o->data.cbegin(), min);

    return dist.distance();
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

PtrIValue StateValue::clone() const
{
    return PtrIValue(new StateValue(*this));
}

}
