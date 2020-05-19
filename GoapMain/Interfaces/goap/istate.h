#ifndef GOAP_IREADSTATE_H
#define GOAP_IREADSTATE_H

#include <memory>
#include <map>
#include <list>
#include <ostream>
#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "ihashable.h"
#include "istatevalue.h"

namespace goap
{
using namespace std;



class IState : public virtual IStringPrintable, public virtual IClonable, public virtual IHashable
{
public:
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;

    typedef map<IStateValue::New, IStateValue::New> map_value2value_type;

    typedef float value_type;
    typedef intptr_t index_type;
    typedef pair<IStateValue::CPtr, IStateValue::Ptr> pair_value;

    virtual IState* remove(const string &str) = 0;
    virtual IState* remove(const IStateValue::CPtr &key) = 0;
    virtual IState* put(const string &str, const string &value) = 0;
    virtual IState* put(const string &str, const IStateValue::Ptr &value) = 0;
    virtual IState* put(const string &str, initializer_list<float> list) = 0;
    virtual IState* put(const string &str, float number) = 0;
    virtual IState* put(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IState* add(const string &str, const string &value) = 0;
    virtual IState* add(const string &str, const IStateValue::Ptr &value) = 0;
    virtual IState* add(const string &str, initializer_list<float> list) = 0;
    virtual IState* add(const string &str, float number) = 0;
    virtual IState* add(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IState* mul(const string &str, const string &value) = 0;
    virtual IState* mul(const string &str, const IStateValue::Ptr &value) = 0;
    virtual IState* mul(const string &str, initializer_list<float> list) = 0;
    virtual IState* mul(const string &str, float number) = 0;
    virtual IState* mul(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IState* addCost(float c) = 0;
    virtual IState* mulCost(float c) = 0;
    virtual IStateValue & atRef(const IStateValue::CPtr &key) const = 0;
    virtual IStateValue & atRef(const string &str) const = 0;
    const IStateValue* at(const IStateValue::CPtr &key) const { return &atRef(key); }
    const IStateValue* at(const string &str) const { return &atRef(str); }
    virtual pair_value at(intptr_t idx) const = 0;
    virtual intptr_t size() const = 0;
    virtual IState* assign(const IState::CPtr &other) = 0;
    virtual IState* assign(const map_value2value_type &map_string_float) = 0;
    virtual bool equals(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual IState* cost(float c) = 0;
};

inline static ostream& operator<<(ostream& os, const list<IState::CPtr>& dt) {
    const char *sz = "[";
    for (auto &action : dt) {
        os << sz << *action;
        sz = ",\n ";
    }
    return os << "]";
}

inline bool operator ==(const IState& a, const IState& b)
{
    return a.equals(IState::CPtr(&b));
}

inline bool operator !=(const IState& a, const IState& b)
{
    return !(a == b);
}

}


namespace std
{
using namespace goap;

template <>
struct hash<IState::CPtr>
{
    size_t operator()(const IState::CPtr &k) const {
        return k->hash();
    }
};

template <>
struct hash<IState::Ptr>
{
    size_t operator()(const IState::Ptr &k) const {
        return k->hash();
    }
};

template<>
struct equal_to<IState::CPtr>
{
    bool operator()(const IState::CPtr &data1, const IState::CPtr &data2) const {
        return data1->equals(data2);
    }
};

}

#endif // GOAP_IREADSTATE_H