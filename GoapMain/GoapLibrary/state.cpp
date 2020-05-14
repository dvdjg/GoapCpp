#include <iterator>
#include "basicmath.h"
#include "state.h"
#include "statevalue.h"

#include "newptr.h"

namespace goap
{

using namespace std;

State::State()
{
}

State::State(const State &other) : _data(other._data), _coste(other._coste)
{
}

State::State(const IState::CPtr &other)
{
    assign(other);
}

State::State(std::initializer_list<data_type::value_type> list) : _data(list)
{

}

void State::clear() {
    _data.clear();
    _coste = 0;
}

void State::assign(const State &o)
{
    _data = o._data;
    _coste = o._coste;
/*
    data_type dt({{NewPtr<IStateValue>({}, "Uno"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})},
                  {NewPtr<IStateValue>({}, "Dos"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})}});
    State st({{NewPtr<IStateValue>({}, "Uno"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})},
              {NewPtr<IStateValue>({}, "Dos"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})}});
*/
}

void State::assign(const IState::CPtr &other)
{
    auto o = dynamic_pointer_cast<const State>(other);
    if (o) {
        assign(*o);
    } else {
        throw new std::runtime_error(__func__);
    }
}
/*
void State::add(const std::string &key, const IStateValue::CPtr &other)
{
    IStateValue::Ptr val = State::at(key);
    val->add
    int len = std::min(val->size(), other->size());
    for (int i = 0; i < len; ++i) {

    }
    _map[key] += value;
    touch();
    return this;
}

void State::mul(const std::string &key, const IStateValue::CPtr &other)
{
    _map[key] *= value;
    touch();
    return this;
}

bool State::and(const std::string &key, bool other)
{
    _map[key] = _map[key] && other;
    touch();
    return this;
}

bool State::or(const std::string &key, bool other)
{
    _map[key] = _map[key] || other;
    touch();
    return this;
}
*/
void State::remove(const IStateValue::CPtr &key)
{
    _data.erase(key);
}

void State::remove(const std::string &str)
{
    remove(NewPtr<IStateValue>({}, str));
}

IState::pair_value State::at(intptr_t idx) const
{
    if (idx < 0 || idx >= intptr_t(_data.size())) {
        throw new std::runtime_error(__func__);
    }
    auto it = std::next(_data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

IStateValue& State::atRef(const IStateValue::CPtr &key) const
{
    auto it = _data.find(key);
    if (it == _data.end()) {
        throw new std::runtime_error(__func__);
    }
    return *it->second;
}

IStateValue& State::atRef(const std::string &str) const
{
    return atRef(NewPtr<IStateValue>({}, str));
}

IState* State::put(const IStateValue::CPtr &key, const IStateValue::Ptr &value) {
    _data[key] = value;
    return this;
}

IState* State::put(const std::string &str, const IStateValue::Ptr &value) {
    return put(NewPtr<IStateValue>({}, str), value);
}

IState* State::put(const std::string &str, const std::string &value) {
    return put(NewPtr<IStateValue>({}, str), NewPtr<IStateValue>({}, value));
}

IState* State::put(const std::string &str, std::initializer_list<float> list) {
    return put(str, NewPtr<IStateValue>({}, list));
}

IState* State::put(const string &str, float number) {
    return put(str, {number});
}


IState* State::add(const IStateValue::CPtr &key, const IStateValue::Ptr &value) {
    *_data[key] += *value;
    return this;
}

IState* State::add(const std::string &str, const IStateValue::Ptr &value) {
    return add(NewPtr<IStateValue>({}, str), value);
}

IState* State::add(const std::string &str, const std::string &value) {
    return add(NewPtr<IStateValue>({}, str), NewPtr<IStateValue>({}, value));
}

IState* State::add(const std::string &str, std::initializer_list<float> list) {
    return add(str, NewPtr<IStateValue>({}, list));
}

IState* State::add(const string &str, float number) {
    return add(str, {number});
}

IState* State::mul(const IStateValue::CPtr &key, const IStateValue::Ptr &value) {
    *_data[key] += *value;
    return this;
}

IState* State::mul(const std::string &str, const IStateValue::Ptr &value) {
    return mul(NewPtr<IStateValue>({}, str), value);
}

IState* State::mul(const std::string &str, const std::string &value) {
    return mul(NewPtr<IStateValue>({}, str), NewPtr<IStateValue>({}, value));
}

IState* State::mul(const std::string &str, std::initializer_list<float> list) {
    return mul(str, NewPtr<IStateValue>({}, list));
}

IState* State::mul(const string &str, float number) {
    return mul(str, {number});
}


intptr_t State::size() const
{
    return intptr_t(_data.size());
}

bool State::equals(const IHashable::CPtr &other) const
{
    return equals(dynamic_pointer_cast<const IState>(other));
}

bool State::equals(const IState::CPtr &other) const
{
    auto o = dynamic_cast<const State*>(other.get());
    return o && basicmath::floatEqual(_coste, o->_coste) && _data == o->_data;
}

void State::assign(const IState::map_string_float_type &map_string_float)
{
    for (auto pair : map_string_float) {
        put(pair.first, {pair.second});
    }
}

float State::cost() const
{
    return _coste;
}

void State::cost(float c)
{
    _coste = c;
}

IState* State::addCost(float c) {
    _coste += c;
    return this;
}

IState* State::mulCost(float c) {
    _coste *= c;
    return this;
}

IClonable::Ptr State::clone() const
{
    auto ptr = NewPtr<IState>({}, *this);
    return std::move(ptr);
}

string State::toDebugString() const
{
    string ret{"{"};
    ret += "coste:";
    ret += std::to_string(_coste);
    ret += ", ";

    for (auto it = _data.cbegin(); it != _data.cend(); ++it) {
        ret += it->first->toString();
        ret += ":";
        ret += it->second->toDebugString();
        ret += ", ";
    }
    ret.pop_back();
    ret.pop_back();
    ret += '}';
    return ret;
}

string State::toString() const
{
    string ret{"{"};
    ret += "coste:";
    ret += std::to_string(_coste);
    ret += ", ";

    for (auto it = _data.cbegin(); it != _data.cend(); ++it) {
        ret += it->first->toString();
        ret += " : ";
        ret += it->second->toString();
        ret += ", ";
    }
    ret.pop_back();
    ret.pop_back();
    ret += '}';
    return ret;
}

size_t State::hash() const
{
    std::size_t h = basicmath::hash(&_coste, 1);
    for (auto &it : _data) {
        std::size_t hkey = it.first->hash();
        std::size_t hvalue = it.second->hash();
        h = h ^ hkey ^ hvalue;
    }
    return h;
}

}
