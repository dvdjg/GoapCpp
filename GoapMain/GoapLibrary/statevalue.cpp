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

StateValue::StateValue(const IStateValue::CPtr &other)
{
    assign(other);
}

StateValue::StateValue(const std::string &str)
{
    fromString(str);
}

StateValue::StateValue( std::initializer_list<float> list) : data(list)
{
}

std::size_t StateValue::size() const
{
    return data.size();
}

void StateValue::resize(std::size_t len)
{
    data.resize(len);
}

float StateValue::atF(float idx) const
{
    if (data.size() == 0) {
        return 0.f;
    }
    float ret = interp2(idx, &data[0], static_cast<int>(data.size()));
    return ret;
}

float StateValue::at(size_t idx) const
{
    if (data.size() == 0) {
        return 0.f;
    } else if (idx >= data.size()) {
        idx = data.size()-1;
    }
    return data.at(idx);
}

void StateValue::fromString(const std::string &str)
{
    data.resize(0);
    std::copy(str.begin(), str.end(), std::back_inserter(data));
}

void StateValue::assign(const std::string &str)
{
    fromString(str);
}

void StateValue::assign(const std::initializer_list<float> &list)
{
    data.assign(list);
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
        interp2array(&o->data[0], o->size(), &data[0], static_cast<int>(size()));
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

void StateValue::setAtF(float idx, float value)
{
//    size_t i = size_t(idx);
//    if (i >= data.size())
//    {
//        throw new std::runtime_error(__func__);
//    }
//    data[i] = value;
    auto i = std::llround(idx);
    if (idx < 0) {
        throw new std::runtime_error(__func__);
    }
    setAt(size_t(i), value);
}
void StateValue::setAt(size_t idx, float value)
{
    //data.insert(data.begin() + static_cast<ssize_t>(idx), value);
    data.at(idx) = value;
}

std::size_t StateValue::hash() const
{
    return basicmath::hash(&data[0], data.size());
}

void StateValue::clear()
{
    data.resize(0);
}

IClonable::Ptr StateValue::clone() const
{
    return new StateValue(*this);
}

void StateValue::assign(const IStateValue::CPtr &other)
{
    data.resize(other->size());
    for (size_t i = 0; i < other->size(); ++i) {
        data[i] = other->at(i);
    }
}

}

