#include <iterator>
#include "state.h"
#include "statevalue.h"

#include "newptr.h"

namespace goap
{

using namespace std;

//std::string State::g_strStateValueRegistration;

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

void State::assign(const IState::CPtr &other)
{
    auto o = dynamic_pointer_cast<const State>(other);
    if (o) {
        data = o->data;
        coste = o->coste;
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
    auto o = dynamic_pointer_cast<const State>(other);
    return o && data == o->data;
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
    return std::move(ptr); //new State(*this);
}

void State::fromString(const string &)
{
}

string State::toDebugString() const
{
    return {};
}

string State::toString() const
{
    return {};
}

}
