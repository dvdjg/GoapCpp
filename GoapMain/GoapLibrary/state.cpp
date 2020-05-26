#include <iterator>
#include <sstream>
#include "termcolor/termcolor.hpp"
#include "basicmath.h"
#include "state.h"
#include "statevalue.h"
#include "log_hook.h"

#include "newptr.h"

namespace goap
{

using namespace std;
using namespace termcolor;

void State::touch() {
    _cachedHash = 0;
}

State::State() {
}

State::State(const State &other) : _data(other._data), _coste(other._coste) {
}

State::State(const IState::CPtr &other) {
    assign(other);
}

State::State(std::initializer_list<data_type::value_type> list) : _data(list) {
}

void State::clear() {
    _data.clear();
    _coste = 1;
    _cachedHash = 0;
}

//IState::IStateIterator::Ptr State::cbegin() const
//{
//    auto ret = NewPtr<IStateIterator>({}, _data.begin());
//    return ret;
//}

//IState::IStateIterator::Ptr State::cend() const
//{
//    auto ret = NewPtr<IStateIterator>({}, _data.end());
//    return ret;
//}

list<IStateValue::CPtr> State::keys() const
{
    list<IStateValue::CPtr> ret;
    for (auto &it : _data) {
        ret.push_back(it.first);
    }
    return ret;
}

IState* State::assign(const State &o) {
    _data = o._data;
    _coste = o._coste;
    touch();
    return this;
}

IState* State::assign(const IState::CPtr &other) {
    auto o = dynamic_pointer_cast<const State>(other);
    if (o) {
        assign(*o);
    } else {
        throw std::runtime_error(__func__);
    }
    return this;
}

IState* State::remove(const IStateValue::CNew &key) {
    _data.erase(key);
    touch();
    return this;
}

IState::pair_value State::at(int64_t idx) const {
    static IStateValue::Ptr nullValue = NewPtr<IStateValue>();
    if (idx < 0 || idx >= int64_t(_data.size())) {
        nullValue->clear();
        return std::make_pair(nullValue, nullValue);
    }
    auto it = std::next(_data.begin(), idx);
    return std::make_pair(it->first, it->second);
}

IStateValue* State::at(const IStateValue::CNew &key) const {
    auto it = _data.find(key);
    if (it == _data.end() || !it->second) {
        return nullptr;
    }
    IStateValue* value = it->second.get();
    return value;
}

IStateValue &State::atRef(const IStateValue::CNew &key) const
{
    IStateValue* pVal = at(key);
    if (!pVal) {
        static IStateValue::Ptr nullValue = NewPtr<IStateValue>();
        nullValue->clear();
        pVal = nullValue.get();
    }
    return *pVal;
}

IState* State::put(const IStateValue::CNew &key, const IStateValue::New &value) {
    _data[key] = value;
    touch();
    return this;
}

IState* State::add(const IStateValue::CNew &key, const IStateValue::New &value) {
    *_data[key] += *value;
    touch();
    return this;
}

IState* State::mul(const IStateValue::CNew &key, const IStateValue::New &value) {
    *_data[key] += *value;
    touch();
    return this;
}

int64_t State::size() const
{
    return int64_t(_data.size());
}

bool State::equals(const IHashable::CPtr &other) const
{
    return equals(dynamic_pointer_cast<const IState>(other));
}

bool State::equals(const IState::CPtr &other) const
{
    auto o = dynamic_cast<const State*>(other.get());
    bool ret = o && _data.size() == o->_data.size(); /*basicmath::floatEqual(cost(), o->cost()) &&*/
    if (ret) {
        //LOG(DEBUG) << *this << " != " << *other << endl;
        for (auto & it : _data) {
            data_type::const_iterator it2 = o->_data.find(it.first);
            if (it2 == o->_data.cend() || !it.second->equals(it2->second)) {
                return false;
            }
        }
    }
    return ret;
}

IState* State::assign(const IState::map_value2value_type &map_string_float)
{
    for (auto pair : map_string_float) {
        put(pair.first, pair.second);
    }
    return this;
}

explicit_ptr<IStateIterator> State::iterator() const
{
    NewPtr<IStateIterator> itState;
    *itState = this;
    return std::move(itState);
}

float State::cost() const
{
    return _coste;
}

IState* State::cost(float c)
{
    _coste = c;
    touch();
    return this;
}

IState* State::addCost(float c) {
    _coste += c;
    touch();
    return this;
}

IState* State::mulCost(float c) {
    _coste *= c;
    touch();
    return this;
}

IClonable::Ptr State::clone() const
{
    auto ptr = NewPtr<IState>();
    ptr->cost(this->cost());
    auto itState = iterator();
    while (itState->hasNext()) {
        auto pair = itState->next();
        IStateValue::CPtr key = pair.first;
        IStateValue::Ptr value = dynamic_pointer_cast<IStateValue>(pair.second->clone());
        ptr->put(key, value);
    }
    return std::move(ptr);
}

string State::toString() const
{
    return toDebugString();
}

string State::toDebugString() const
{
    std::stringstream ss;
    toOstream(ss);
    string str = ss.str();
    return str;
}

ostream &State::toOstream(ostream &ss) const
{
    ss << '{' << "coste:" << _coste;
    for (auto it = _data.cbegin(); it != _data.cend(); ++it) {
        ss << ", " << green << it->first->toString() << reset << " : " << magenta << it->second->toString() << reset;
    }
    ss << '}';
    return ss;
}

size_t State::hash() const
{
    if (_cachedHash == 0) {
        std::size_t h = basicmath::hash(&_coste, 1);
        for (auto &it : _data) {
            std::size_t hkey = it.first->hash();
            std::size_t hvalue = it.second->hash();
            h = h ^ hkey ^ hvalue;
        }
        if (h == 0) {
            h = 1; // Do not return 0
        }
        _cachedHash = h;
    }
    return _cachedHash;
}

bool State::empty() const
{
    return _data.empty();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
IState::New::New() : parent_type(NewPtr<IState>()) {
}

IState::New::New(parent_type::element_type *pVal) : parent_type(pVal) {
}

IState::New::New(const parent_type &other) : parent_type(other) {
}

IState::New::New(const map_value2value_type &mapValueValue) : New() {
    get()->assign(mapValueValue);
}

IState::New::New(initializer_list<map_value2value_type::value_type> mapValueValue) : New() {
    get()->assign(mapValueValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
IState::CNew::CNew() : parent_type(NewPtr<IState>()) {
}

IState::CNew::CNew(parent_type::element_type *pVal) : parent_type(pVal) {
}

IState::CNew::CNew(const parent_type &other) : parent_type(other) {
}

IState::CNew::CNew(const class_type::Ptr &other) : parent_type(other) {
    // TODO: Clone for an inmutable version?
}

IState::CNew::CNew(const map_value2value_type &mapValueValue) : CNew(New(mapValueValue)) {
}

IState::CNew::CNew(initializer_list<map_value2value_type::value_type> mapValueValue) : CNew(New(mapValueValue)) {
}

}
