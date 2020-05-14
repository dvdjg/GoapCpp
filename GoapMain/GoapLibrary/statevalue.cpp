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

IStringValue* StateValue::fromString(const std::string &str)
{
    return assign(str);
}

IStringValue* StateValue::assign(const char* str)
{
    _data.resize(0);
    for (const char *it = str; *it; ++it) {
        _data.push_back(*it);
    }
    return this;
}

IStringValue* StateValue::assign(const std::string &str)
{
    _data.resize(0);
    std::copy(str.begin(), str.end(), std::back_inserter(_data));
    return this;
}

IStringValue* StateValue::assign(const std::initializer_list<float> &list)
{
    _data.assign(list);
    return this;
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

void StateValue::put(intptr_t idx, float value)
{
    if (idx < 0) {
        throw new std::runtime_error(__func__);
    }
    _data.at(std::size_t(idx)) = value;
}

void StateValue::putAll(float value)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        put(i, value);
    }
}

void StateValue::put(float idx, float value)
{
    auto i = std::llround(idx);
    if (i < 0) {
        throw new std::runtime_error(__func__);
    }
    put(intptr_t(i), value);
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
}

IStringValue* StateValue::assign(const StateValue &other)
{
    _data = other._data;
    return this;
}

IStringValue* StateValue::assign(const IStateValue::CPtr &other)
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
    return this;
}

bool StateValue::equals(const IStateValue::CPtr &other) const
{
    bool ret(other);
    if (ret) {
        auto thisSize = size();
        ret = thisSize == other->size();
        for (int_type i = 0; ret && i < int_type(thisSize); ++i) {
            ret = basicmath::floatEqual(at(i), other->at(i));
        }
    }
    return ret;
}

bool StateValue::equals(const IHashable::CPtr &other) const
{
    return equals(dynamic_pointer_cast<const IStateValue>(other));
}

bool StateValue::equals(const std::string &other) const
{
    bool ret(true);
    auto thisSize = size();
    ret = thisSize == int_type(other.size());
    for (int_type i = 0; ret && i < int_type(thisSize); ++i) {
        ret = basicmath::floatEqual(at(i), other.at(std::size_t(i)));
    }
    return ret;
}

bool StateValue::equals(const char *str) const
{
    auto it = _data.cbegin();
    const char *pc = nullptr;
    for (pc = str; *pc && it != _data.cend(); ++pc, ++it) {
        if (!basicmath::floatEqual(*it, *pc)) {
            return false;
        }
    }
    return !*pc && it == _data.cend();
}

bool StateValue::equals(const std::initializer_list<float> &other) const
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

void StateValue::add(const IStateValue::CPtr &other)
{
    auto thisSize = size();
    auto otherSize = other->size();
    if (otherSize > thisSize) {
        resize(otherSize);
    }
    for (int_type i = 0; i < otherSize; ++i) {
        float thisValue = at(i);
        float otherValue = other->at(i);
        put(i, thisValue + otherValue);
    }
}

void StateValue::mul(const IStateValue::CPtr &other)
{
    auto thisSize = size();
    auto otherSize = other->size();
    for (int_type i = 0; i < thisSize; ++i) {
        if (i >= otherSize) {
            put(i, 0); // Asumir ceros en los valores que faltan
        } else {
            float thisValue = at(i);
            float otherValue = other->at(i);
            put(i, thisValue * otherValue);
        }
    }
}

void StateValue::and_logic(const IStateValue::CPtr &other)
{
    auto thisSize = size();
    auto otherSize = other->size();
    for (int_type i = 0; i < thisSize; ++i) {
        if (i >= otherSize) {
            put(i, 0); // Asumir ceros en los valores que faltan
        } else {
            float thisValue = at(i);
            float otherValue = other->at(i);
            put(i, thisValue != 0 && otherValue != 0);
        }
    }
}

void StateValue::or_logic(const IStateValue::CPtr &other)
{
    auto thisSize = size();
    auto otherSize = other->size();
    for (int_type i = 0; i < thisSize; ++i) {
        if (i >= otherSize) {
            break; // Asumir ceros en los valores que faltan
        } else {
            float thisValue = at(i);
            float otherValue = other->at(i);
            put(i, thisValue != 0 || otherValue != 0);
        }
    }
}

void StateValue::add(float other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue + other);
    }
}

void StateValue::mul(float other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue * other);
    }
}

void StateValue::and_logic(bool other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue != 0 && other);
    }
}

void StateValue::or_logic(bool other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue != 0 || other);
    }
}

}

