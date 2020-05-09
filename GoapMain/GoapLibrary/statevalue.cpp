#include <stdexcept>
#include <algorithm>
#include "statevalue.h"
#include "basicmath.h"
#include "cosinedistance.h"

#include "newptr.h"

namespace goap
{

using namespace std;
using namespace basicmath;

StateValue::StateValue()
{
}

StateValue::StateValue(const StateValue &other) : _data(other._data)
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

StateValue::StateValue(std::initializer_list<float> list) : _data(list)
{
}

intptr_t StateValue::size() const
{
    return intptr_t(_data.size());
}

void StateValue::resize(intptr_t len)
{
    _data.resize(std::size_t(len));
}

float StateValue::at(float idx) const
{
    if (_data.size() == 0) {
        return 0.f;
    }
    float ret = interp2(idx, &_data[0], static_cast<int>(_data.size()));
    return ret;
}

float StateValue::at(intptr_t idx) const
{
    if (_data.size() == 0) { // || idx >= intptr_t(data.size()) || idx < 0) {
        return 0.f;
    } else if (idx >= intptr_t(_data.size())) {
        idx = intptr_t(_data.size()-1);
    } else if (idx < 0) {
        idx = 0;
    }
    return _data.at(std::size_t(idx));
}

//float StateValue::at(int idx) const
//{
//    if (idx < 0) {
//        idx = 0;
//    }
//    return this->at(size_t(idx));
//}

void StateValue::fromString(const std::string &str)
{
    _data.resize(0);
    std::copy(str.begin(), str.end(), std::back_inserter(_data));
}

void StateValue::assign(const std::string &str)
{
    fromString(str);
}

void StateValue::assign(const std::initializer_list<float> &list)
{
    _data.assign(list);
}

void StateValue::interpolateFrom(const IStateValue::CPtr &other)
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (!o) {
        throw new std::runtime_error(__func__);
    }

    if (size() > 0 && &o->_data[0] != &_data[0]) {
        interp2array(&o->_data[0], int_type(o->size()), &_data[0], int_type(size()));
    }
}

float StateValue::cosineDistance(const IStateValue::CPtr &other, float *pThisModule, float *pOthersModule) const
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (!o) {
        throw new std::runtime_error(__func__);
    }
    size_t min = std::min(_data.size(), o->_data.size());

    auto cosDist = fnCosineDistance(_data.cbegin(), o->_data.cbegin(), min);
    if (pThisModule) {
        *pThisModule = cosDist.aModule();
    }
    if (pOthersModule) {
        *pOthersModule = cosDist.bModule();
    }
    return cosDist.distance();
}

std::string StateValue::toDebugString() const
{
    std::string ret{"["};

    for(auto &it : _data) {
        ret += std::to_string(it);
        ret += ", ";
    }
    if (ret.size() >= 2) {
        ret.pop_back();
        ret.pop_back();
    }
    ret += ']';
    return ret;
}

std::string StateValue::toString() const
{
    std::string ret;
    for(auto &it : _data) {
        ret += static_cast<char>(it);
    }
    return ret;
}

void StateValue::setAt(intptr_t idx, float value)
{
    if (idx < 0) {
        throw new std::runtime_error(__func__);
    }
    _data.at(std::size_t(idx)) = value;
}

void StateValue::setAt(float idx, float value)
{
    auto i = std::llround(idx);
    if (i < 0) {
        throw new std::runtime_error(__func__);
    }
    setAt(intptr_t(i), value);
}

//void StateValue::setAt(size_t idx, float value)
//{
//    //data.insert(data.begin() + static_cast<ssize_t>(idx), value);
//    data.at(idx) = value;
//}

std::size_t StateValue::hash() const
{
    return basicmath::hash(&_data[0], _data.size());
}

void StateValue::clear()
{
    _data.resize(0);
}

IClonable::Ptr StateValue::clone() const
{
    auto ptr = NewPtr<IStateValue>({}, *this);
    return std::move(ptr);
    //return new StateValue(*this);
}

void StateValue::assign(const StateValue &other)
{
    _data = other._data;
}

void StateValue::assign(const IStateValue::CPtr &other)
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (!o) {
        _data = o->_data;
    } else {
        _data.resize(std::size_t(other->size()));
        for (int_type i = 0; i < int_type(other->size()); ++i) {
            _data.at(std::size_t(i)) = other->at(i);
        }
    }
}

bool StateValue::equal(const IStateValue::CPtr &other) const
{
    bool ret(other);
    if (ret)
    {
        auto thisSize = size();
        ret = thisSize == other->size();
        for (int_type i = 0; ret && i < int_type(thisSize); ++i) {
            ret = basicmath::floatEqual(at(i), other->at(i));
        }
    }
    return ret;
}

bool StateValue::equal(const IHashable::CPtr &other) const
{
    return equal(dynamic_pointer_cast<const IStateValue>(other));
}

bool StateValue::equal(const std::string &other) const
{
    bool ret(true);
    auto thisSize = size();
    ret = thisSize == int_type(other.size());
    for (int_type i = 0; ret && i < int_type(thisSize); ++i) {
        ret = basicmath::floatEqual(at(i), other.at(std::size_t(i)));
    }
    return ret;
}


bool StateValue::equal(const std::initializer_list<float> &other) const
{
    bool ret(true);
    auto thisSize = size();
    ret = thisSize == int_type(other.size());
    auto beg = other.begin();
    for (int_type i = 0; ret && i < int_type(thisSize); ++i, ++beg) {
        ret = basicmath::floatEqual(at(i), *beg);
    }
    return ret;
}


}

