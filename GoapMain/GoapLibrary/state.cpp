#include <iterator>
#include "state.h"
#include "statevalue.h"

namespace goap
{

State::State()
{
}

void State::remove(const PtrIStateValue &key)
{
    data.erase(key);
}

PtrIStateValue State::at(const PtrIStateValue &key) const
{
    return data.at(key);
}

void State::setAt(const PtrIStateValue &key, const PtrIStateValue &value)
{
    data[key] = value;
}

size_t State::size() const
{
    return data.size();
}

bool State::equals(const IState *other) const
{
    auto o = dynamic_cast<const State*>(other);
    return data == o->data;
}

float State::cost() const
{
    return coste;
}

void State::setCost(float c)
{
    coste = c;
}

std::pair<PtrIStateValue, PtrIStateValue> State::at(intptr_t idx) const
{
    auto it = std::next(data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

void State::remove(const std::string &str)
{
    remove(PtrIStateValue(new StateValue(str)));
}

void State::setAt(const std::string &str, const PtrIStateValue &value)
{
    setAt(PtrIStateValue(new StateValue(str)), value);
}

PtrIStateValue State::at(const std::string &str) const
{
    return at(PtrIStateValue(new StateValue(str)));
}

}
