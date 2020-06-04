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

class IStateValueIterator;

class IState : public virtual IStringPrintable, public virtual IClonable, public virtual IHashable
{
public:
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;

    typedef map<IStateValue::CNew, IStateValue::New> map_value2value_type;

    typedef IState class_type;
    typedef int64_t index_type;
    typedef pair<float, IState::CPtr> pair_state; // Sequence Position / State
    typedef pair<IStateValue::CPtr, IStateValue::Ptr> pair_value;


    class New : public class_type::Ptr {
        typedef class_type::Ptr parent_type;
    public:
        New();
        New(parent_type::element_type* pVal);
        New(const parent_type &other);
        New(const map_value2value_type &mapValueValue);
        New(initializer_list<map_value2value_type::value_type> mapValueValue);
    };

    class CNew : public class_type::CPtr {
        typedef class_type::CPtr parent_type;
    public:
        CNew();
        CNew(const class_type::Ptr &other);
        CNew(const parent_type::element_type* pVal);
        CNew(const parent_type &other);
        CNew(const map_value2value_type &mapValueValue);
        CNew(initializer_list<map_value2value_type::value_type> mapValueValue);
    };

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
    virtual IState* assign(const IState &other) { return assign(&other); }
    virtual IState* assign(const map_value2value_type &map_string_float) = 0;
    virtual IState* merge(const IState::CNew& other) = 0;
    virtual bool equals(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual IState* cost(float c) = 0;
    virtual explicit_ptr<IStateValueIterator> iterator() const = 0;
};


class IStateValueIterator : public IRefCounter {
public:
    typedef explicit_ptr<IStateValueIterator> Ptr;
    typedef explicit_ptr<const IStateValueIterator> CPtr;

    virtual bool hasNext() const = 0;
    virtual IState::pair_value next() = 0;
    virtual IState::pair_value peekNext() = 0;
    virtual int64_t size() const = 0;
    virtual IStateValueIterator* operator=(const IState::CPtr &container) = 0;
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
template<>
struct equal_to<IState::Ptr>
{
    bool operator()(const IState::Ptr &data1, const IState::CPtr &data2) const {
        return data1->equals(data2);
    }
};

template <>
struct hash<IState::CNew>
{
    size_t operator()(const IState::CNew &k) const {
        return k->hash();
    }
};

template <>
struct hash<IState::New>
{
    size_t operator()(const IState::New &k) const {
        return k->hash();
    }
};

template<>
struct equal_to<IState::CNew>
{
    bool operator()(const IState::CNew &data1, const IState::CPtr &data2) const {
        return data1->equals(data2);
    }
};
template<>
struct equal_to<IState::New>
{
    bool operator()(const IState::New &data1, const IState::CPtr &data2) const {
        return data1->equals(data2);
    }
};

}

#endif // GOAP_IREADSTATE_H
