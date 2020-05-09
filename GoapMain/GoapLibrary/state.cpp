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

void State::remove(const IStateValue::CPtr &key)
{
    _data.erase(key);
}

void State::remove(const std::string &str)
{
    remove(NewPtr<IStateValue>({}, str)); // IStateValue::CPtr(new StateValue(str))
}

IStateValue::Ptr State::at(const IStateValue::CPtr &key) const
{
    auto it = _data.find(key);
    return (it == _data.end()) ? IStateValue::Ptr() : it->second;
}

IState::pair_value State::at(intptr_t idx) const
{
    if (idx < 0 || idx >= intptr_t(_data.size())) {
        throw new std::runtime_error(__func__);
    }
    auto it = std::next(_data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

IStateValue::Ptr State::at(const std::string &str) const
{
    return at(NewPtr<IStateValue>({}, str));
}

void State::setAt(const IStateValue::CPtr &key, const IStateValue::Ptr &value)
{
    _data[key] = value;
}

void State::setAt(const std::string &str, const IStateValue::Ptr &value)
{
    setAt(NewPtr<IStateValue>({}, str), value);
}

void State::setAt(const std::string &str, std::initializer_list<float> list)
{
    NewPtr<IStateValue> val({}, list);
    setAt(NewPtr<IStateValue>({}, str), val);
}

intptr_t State::size() const
{
    return intptr_t(_data.size());
}

bool State::equal(const IHashable::CPtr &other) const
{
    return equal(dynamic_pointer_cast<const IState>(other));
}

bool State::equal(const IState::CPtr &other) const
{
    auto o = dynamic_cast<const State*>(other.get());
    return o && basicmath::floatEqual(_coste, o->_coste) && _data == o->_data;
}

float State::cost() const
{
    return _coste;
}

void State::cost(float c)
{
    _coste = c;
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
