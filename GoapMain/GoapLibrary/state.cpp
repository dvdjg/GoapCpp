#include <iterator>
#include "state.h"
#include "statevalue.h"

State::State()
{
}

void State::remove(const ptr_value &key)
{
    data.erase(key);
}

ptr_value State::at(const ptr_value &key) const
{
    return data.at(key);
}

void State::setAt(const ptr_value &key, const ptr_value &value)
{
    data[key] = value;
}

size_t State::size() const
{
    data.size();
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

std::pair<ptr_value, ptr_value> State::at(ssize_t idx) const
{
    auto it = std::next(data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

void State::remove(const std::u16string &str)
{
    remove(ptr_value(new StateValue(str)));
}

void State::setAt(const std::u16string &str, const ptr_value &value)
{
    setAt(ptr_value(new StateValue(str)), value);
}

ptr_value State::at(const std::u16string &str) const
{
    return at(ptr_value(new StateValue(str)));
}
