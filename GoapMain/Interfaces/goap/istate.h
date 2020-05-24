#ifndef GOAP_IREADSTATE_H
#define GOAP_IREADSTATE_H

//#include <memory>
#include <map>
#include <list>
#include <ostream>
#include "common/irefcounter.h"
#include "ihashable.h"
#include "istatevalue.h"
#include "explicit_ptr.h"

namespace goap
{
using namespace std;

class IStateIterator;

class IState : public virtual IStringPrintable, public virtual IClonable, public virtual IHashable
{
public:
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;

    typedef map<IStateValue::CNew, IStateValue::New> map_value2value_type;

    typedef float value_type;
    typedef int64_t index_type;
    typedef pair<IStateValue::CPtr, IStateValue::Ptr> pair_value;

    virtual list<IStateValue::CPtr> keys() const = 0;
    virtual IState* remove(const IStateValue::CNew &key) = 0;
    virtual IState* put(const IStateValue::CNew &key, const IStateValue::New &value) = 0;
    virtual IState* add(const IStateValue::CNew &key, const IStateValue::New &value) = 0;
    virtual IState* mul(const IStateValue::CNew &key, const IStateValue::New &value) = 0;
    virtual IState* addCost(float c) = 0;
    virtual IState* mulCost(float c) = 0;
    virtual IStateValue & atRef(const IStateValue::CNew &key) const = 0;
    virtual IStateValue* at(const IStateValue::CNew &key) const = 0;
    virtual pair_value at(int64_t idx) const = 0;
    virtual pair_value at(int idx) const { return at(static_cast<int64_t>(idx));}
    virtual int64_t size() const = 0;
    virtual bool empty() const { return size() == 0; }
    virtual IState* assign(const IState::CPtr &other) = 0;
    virtual IState* assign(const map_value2value_type &map_string_float) = 0;
    virtual bool equals(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual IState* cost(float c) = 0;
    virtual explicit_ptr<IStateIterator> iterator() const = 0;
};


class IStateIterator : public IRefCounter {
public:
    typedef explicit_ptr<IStateIterator> Ptr;
    typedef explicit_ptr<const IStateIterator> CPtr;

    virtual bool hasNext() const = 0;
    virtual IState::pair_value next() = 0;
    virtual IStateIterator* operator=(const IState::CPtr &container) = 0;
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
