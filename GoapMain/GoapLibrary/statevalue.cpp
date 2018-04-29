#include <stdexcept>
#include <algorithm>
#include "statevalue.h"
#include "basicmath.h"

namespace goap
{

using namespace std;
using namespace basicmath;

StateValue::StateValue()
{
}

StateValue::StateValue(const StateValue &other) : data(other.data)
{
}

StateValue::StateValue(const std::string &str)
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
    float ret = interp2(idx, &data[0], static_cast<int>(data.size()));
    return ret;
}

void StateValue::fromString(const std::string &str)
{
    data.resize(0);
    std::copy(str.begin(), str.end(), std::back_inserter(data));
}

void StateValue::interpolateFrom(const IStateValue::CPtr &other)
{
    auto o = dynamic_pointer_cast<const StateValue>(other); // dynamic_pointer_cast<const StateValue>(other); // dynamic_cast<const StateValue *>(other);
    if (!o)
    {
        throw new std::runtime_error(__func__);
    }
    resize(o->size());
    if (size() > 0)
    {
        interp2array(&o->data[0], o->size(), &data[0], size());
    }
}

float StateValue::cosineDistance(const IStateValue::CPtr &other) const
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (!o)
    {
        throw new std::runtime_error(__func__);
    }
    size_t min = std::min(data.size(), o->data.size());

    auto dist = fnCosineDistance(data.cbegin(), o->data.cbegin(), min);

    return dist.distance();
}

std::string StateValue::toString() const
{
    std::string ret(data.begin(), data.end());
    return ret;
}

void StateValue::setAt(float idx, float value)
{
    size_t i = size_t(idx);
    data[i] = value;
}

std::size_t StateValue::hash() const
{
    return basicmath::hash(&data[0], data.size());
}

IClonable::Ptr StateValue::clone() const
{
    return new StateValue(*this);
}

}
