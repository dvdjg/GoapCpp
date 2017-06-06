#include <iterator>
#include "state.h"
#include "statevalue.h"

namespace goap
{

State::State()
{
}

void State::remove(const PtrIValue &key)
{
    data.erase(key);
}

PtrIValue State::at(const PtrIValue &key) const
{
    return data.at(key);
}

void State::setAt(const PtrIValue &key, const PtrIValue &value)
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

std::pair<PtrIValue, PtrIValue> State::at(ssize_t idx) const
{
    auto it = std::next(data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

void State::remove(const std::u16string &str)
{
    remove(PtrIValue(new StateValue(str)));
}

void State::setAt(const std::u16string &str, const PtrIValue &value)
{
    setAt(PtrIValue(new StateValue(str)), value);
}

PtrIValue State::at(const std::u16string &str) const
{
    return at(PtrIValue(new StateValue(str)));
}

}
