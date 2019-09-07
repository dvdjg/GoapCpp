#include <stdexcept>
#include <algorithm>
#include "statevalue.h"
#include "basicmath.h"
#include "cosinedistance.h"

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

StateValue::StateValue(std::initializer_list<float> list) : data(list)
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

float StateValue::at(float idx) const
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

float StateValue::at(int idx) const
{
    if (idx < 0) {
        idx = 0;
    }
    return this->at(size_t(idx));
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

    if (size() > 0 && &o->data[0] != &data[0])
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
    std::string ret{"["};

    for(auto &it : data) {
        ret += std::to_string(it);
        ret += ", ";
    }
    if (ret.size() > 2) {
        ret.pop_back();
        ret.pop_back();
    }
    ret += ']';
    return ret;
}

std::string StateValue::toCharacterString() const
{
    std::string ret;
    for(auto &it : data) {
        ret += static_cast<char>(it);
    }
    return ret;
}

void StateValue::setAt(int idx, float value)
{
    if (idx < 0) {
        throw new std::runtime_error(__func__);
    }
    setAt(size_t(idx), value);
}
void StateValue::setAt(float idx, float value)
{
    auto i = std::llround(idx);
    if (i < 0) {
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

bool StateValue::equal(const IStateValue::CPtr &other) const
{
    bool ret(other);
    if (ret)
    {
        std::size_t thisSize = size();
        ret = thisSize == other->size();
        for (std::size_t i = 0; ret && i < thisSize; ++i) {
            ret = floatEqual(at(i), other->at(i));
        }
    }
    return ret;
}
}

