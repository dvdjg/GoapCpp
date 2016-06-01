#include <iterator>
#include "state.h"

State::State()
{

}

void State::remove(const ptr_value &key) const
{
}

ptr_value State::at(const ptr_value &key) const
{
    return data.at(key);
}

ptr_value &State::setAt(const ptr_value &key, const ptr_value &value)
{
    return data[key] = value;
}

size_t State::size() const
{
}

bool State::equals(const IState *other) const
{
}

float State::cost() const
{
}

float &State::setCost(float c) const
{
}


std::pair<ptr_value, ptr_value> State::at(ssize_t idx) const
{
    auto it = std::next(data.begin(), idx);
    return std::make_pair(it->first, it->second);
}
