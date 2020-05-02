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

State::State(const State &other) : data(other.data), coste(other.coste)
{
}

State::State(const IState::CPtr &other)
{
    assign(other);
}

State::State(std::initializer_list<data_type::value_type> list) : data(list)
{

}

void State::assign(const State &o)
{
    data = o.data;
    coste = o.coste;

    data_type dt({{NewPtr<IStateValue>({}, "Uno"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})},
                  {NewPtr<IStateValue>({}, "Dos"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})}});
    State st({{NewPtr<IStateValue>({}, "Uno"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})},
              {NewPtr<IStateValue>({}, "Dos"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})}});

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
    data.erase(key);
}

void State::remove(const std::string &str)
{
    remove(NewPtr<IStateValue>({}, str)); // IStateValue::CPtr(new StateValue(str))
}

IStateValue::Ptr State::at(const IStateValue::CPtr &key) const
{
    return data.at(key);
}

IState::pair_value State::at(intptr_t idx) const
{
    if (idx < 0 || idx >= intptr_t(data.size())) {
        throw new std::runtime_error(__func__);
    }
    auto it = std::next(data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

IStateValue::Ptr State::at(const std::string &str) const
{
    return data.at(NewPtr<IStateValue>({}, str));
}

void State::setAt(const IStateValue::CPtr &key, const IStateValue::Ptr &value)
{
    data[key] = value;
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
    return intptr_t(data.size());
}

bool State::equal(const IState::CPtr &other) const
{
    auto o = dynamic_cast<const State*>(other.get());
    return o && basicmath::floatEqual(coste, o->coste) && data == o->data;
}

float State::cost() const
{
    return coste;
}

void State::setCost(float c)
{
    coste = c;
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
    ret += std::to_string(coste);
    ret += ", ";

    for (auto it = data.cbegin(); it != data.cend(); ++it) {
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
    ret += std::to_string(coste);
    ret += ", ";

    for (auto it = data.cbegin(); it != data.cend(); ++it) {
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

}
