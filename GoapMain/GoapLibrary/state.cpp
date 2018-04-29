#include <iterator>
#include "state.h"
#include "statevalue.h"

namespace goap
{

using namespace std;

State::State()
{
}

void State::remove(const IStateValue::Ptr &key)
{
    data.erase(key);
}

IStateValue::Ptr State::at(const IStateValue::Ptr &key) const
{
    return data.at(key);
}

void State::setAt(const IStateValue::Ptr &key, const IStateValue::Ptr &value)
{
    data[key] = value;
}

size_t State::size() const
{
    return data.size();
}

bool State::equals(const IState::CPtr &other) const
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

std::pair<IStateValue::Ptr, IStateValue::Ptr> State::at(intptr_t idx) const
{
    auto it = std::next(data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

void State::remove(const std::string &str)
{
    remove(IStateValue::Ptr(new StateValue(str)));
}

void State::setAt(const std::string &str, const IStateValue::Ptr &value)
{
    setAt(IStateValue::Ptr(new StateValue(str)), value);
}

IStateValue::Ptr State::at(const std::string &str) const
{
    return at(IStateValue::Ptr(new StateValue(str)));
}

}
