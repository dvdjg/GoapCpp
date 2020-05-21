#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "termcolor/termcolor.hpp"
#include "statevalue.h"
#include "basicmath.h"
#include "cosinedistance.h"

#include "newptr.h"

namespace goap
{

using namespace std;
using namespace basicmath;
using namespace termcolor;

StateValue::StateValue()
{
}

StateValue::StateValue(const StateValue &other)
{
    assign(other);
}

StateValue::StateValue(const IStateValue::CNew &other)
{
    assign(other);
}

StateValue::StateValue(const string &str)
{
    assign(str);
}

StateValue::StateValue(const char *other)
{
    assign(other);
}

StateValue::StateValue(std::initializer_list<float> list) : _data(list)
{
    touch();
}

int64_t StateValue::size() const
{
    return int64_t(_data.size());
}

void StateValue::resize(int64_t len)
{
    _data.resize(std::size_t(len));
    touch();
}

float StateValue::at(float idx) const
{
    if (_data.size() == 0) {
        return 0.f;
    }
    float ret = interp2(idx, &_data[0], static_cast<int64_t>(_data.size()));
    return ret;
}

float StateValue::at(int64_t idx) const
{
    if (_data.size() == 0) { // || idx >= int64_t(data.size()) || idx < 0) {
        return 0.f;
    } else if (idx >= int64_t(_data.size())) {
        idx = int64_t(_data.size()-1);
    } else if (idx < 0) {
        idx = 0;
    }
    return _data.at(std::size_t(idx));
}

IStringValue* StateValue::fromString(const std::string &str)
{
    return assign(str);
}

IStringValue* StateValue::assign(const StateValue &other)
{
    _data = other._data;
    touch();
    return this;
}

IStringValue* StateValue::assign(const IStateValue::CNew &other)
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (o) {
        _data = o->_data;
    } else {
        _data.resize(std::size_t(other->size()));
        for (int_type i = 0; i < int_type(other->size()); ++i) {
            _data.at(std::size_t(i)) = other->at(i);
        }
    }
    touch();
    return this;
}


IStringValue* StateValue::assign(const char* str)
{
    _data.resize(0);
    for (const char *it = str; *it; ++it) {
        _data.push_back(*it);
    }
    touch();
    return this;
}

IStringValue* StateValue::assign(const std::string &str)
{
    _data.resize(0);
    std::copy(str.begin(), str.end(), std::back_inserter(_data));
    touch();
    return this;
}

IStringValue* StateValue::assign(const std::initializer_list<float> &list)
{
    _data.assign(list);
    touch();
    return this;
}

void StateValue::interpolateFrom(const IStateValue::CNew &other)
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (!o) {
        throw std::runtime_error(__func__);
    }

    if (size() > 0 && &o->_data[0] != &_data[0]) {
        interp2array(&o->_data[0], int_type(o->size()), &_data[0], int_type(size()));
    }
    touch();
}

float StateValue::cosineDistance(const IStateValue::CNew &other, float *pThisModule, float *pOthersModule) const
{
    auto o = dynamic_pointer_cast<const StateValue>(other);
    if (!o) {
        throw std::runtime_error(__func__);
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
    std::stringstream ss;
    ss << "[";
    const char *sz = "";
    for(auto value : _data) {
        ss << sz;
        ss << value;
        sz = ", ";
    }
    ss << ']' << ends;
    std::string str = ss.str();
    return str;
}

std::string StateValue::toString() const
{
    std::stringstream ss;
    toOstream(ss);
    string str = ss.str();
    return str;
}


ostream &StateValue::toOstream(ostream &ss) const
{
    ss << green; // << colorize
#ifdef GOAP_DEBUG
    if (!_strDebug.empty()) {
        ss << _strDebug;
        return ss << reset;
    }
#endif
    string str;
    for(float value : _data) {
        float whole, fractional;
        fractional = std::modf(value, &whole);
        if (value < -128 || value >= 128 || (value >= 0 && value < 32) || fractional > 0) {
            ss << toDebugString();
            str.erase();
            break;
        }
        str.push_back(static_cast<char>(value));
    }
    ss << str << reset;
    return ss;
}

void StateValue::put(int64_t idx, float value)
{
    if (idx < 0) {
        throw std::runtime_error(__func__);
    }
    _data.at(std::size_t(idx)) = value;
    touch();
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
        throw std::runtime_error(__func__);
    }
    put(int64_t(i), value);
}

std::size_t StateValue::hash() const
{
    //std::size_t h1 = std::hash<std::string>{}(std::string());
    if (_cachedHash == 0) {
        _cachedHash = basicmath::hash(&_data[0], _data.size());
        if (_cachedHash == 0) {
            _cachedHash = 1; // Do not return 0
        }
    }
    return _cachedHash;
}

void StateValue::clear()
{
#ifdef GOAP_DEBUG
    _strDebug.clear();
#endif
    _cachedHash = 0;
    _data.resize(0);
}

IClonable::Ptr StateValue::clone() const
{
    auto ptr = NewPtr<IStateValue>({}, *this);
    return std::move(ptr);
}

bool StateValue::equals(const IStateValue::CNew &other) const
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
    IStateValue::CNew ptr = dynamic_pointer_cast<const IStateValue>(other);
    return equals(ptr);
}

//bool StateValue::equals(const IStateValue::New &other) const
//{
//    return equals(dynamic_pointer_cast<const IStateValue>(other));
//}

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
        float f1 = at(i);
        float f2 = *beg;
        ret = basicmath::floatEqual(f1, f2);
    }
    return ret;
}

void StateValue::add(const IStateValue::CNew &other)
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
    touch();
}

void StateValue::mul(const IStateValue::CNew &other)
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
    touch();
}

void StateValue::and_logic(const IStateValue::CNew &other)
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
    touch();
}

void StateValue::or_logic(const IStateValue::CNew &other)
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
    touch();
}

void StateValue::add(float other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue + other);
    }
    touch();
}

void StateValue::mul(float other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue * other);
    }
    touch();
}

void StateValue::and_logic(bool other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue != 0 && other);
    }
    touch();
}

void StateValue::or_logic(bool other)
{
    auto thisSize = size();
    for (int_type i = 0; i < thisSize; ++i) {
        float thisValue = at(i);
        put(i, thisValue != 0 || other);
    }
    touch();
}

void StateValue::touch() {
#ifdef GOAP_DEBUG
    _strDebug = toString();
    if (!_strDebug.back()) {
        _strDebug.pop_back();
    }
#endif
    _cachedHash = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
IStateValue::New::New() : parent_type(NewPtr<IStateValue>()) {
}

IStateValue::New::New(parent_type::element_type *pVal) : parent_type(pVal) {
}

IStateValue::New::New(const parent_type &other) : parent_type(other) {
}

IStateValue::New::New(const string &str) : New() {
    get()->assign(str);
}

IStateValue::New::New(const char *sz) : New() {
    get()->assign(sz);
}

IStateValue::New::New(const initializer_list<float> &list) : New() {
    get()->assign(list);
}

IStateValue::New::New(float val) : New(initializer_list<float>{val}) {
}

IStateValue::New::New(int64_t val) : New(static_cast<float>(val)) {
}

IStateValue::New::New(int val) : New(static_cast<float>(val)) {
}

////////////////////////////////////////////////////////////////////////////////////////////////////
IStateValue::CNew::CNew() : parent_type(NewPtr<IStateValue>()) {
}

IStateValue::CNew::CNew(parent_type::element_type *pVal) : parent_type(pVal) {
}

IStateValue::CNew::CNew(const parent_type &other) : parent_type(other) {
}

IStateValue::CNew::CNew(const class_type::Ptr &other) : parent_type(other) {
    // TODO: Clone for an inmutable version?
}

IStateValue::CNew::CNew(const string &str) : CNew(New(str)) {
}

IStateValue::CNew::CNew(const char *sz) : CNew(New(sz)) {
}

IStateValue::CNew::CNew(const initializer_list<float> &list) : CNew(New(list)) {
}

IStateValue::CNew::CNew(float val) : CNew(initializer_list<float>{val}) {
}

IStateValue::CNew::CNew(int64_t val) : CNew(static_cast<float>(val)) {
}

IStateValue::CNew::CNew(int val) : CNew(static_cast<float>(val)) {
}


}

