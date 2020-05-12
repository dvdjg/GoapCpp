#ifndef GOAP_IREADSTATE_H
#define GOAP_IREADSTATE_H

#include <memory>
#include <map>
#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "ihashable.h"
#include "istatevalue.h"

namespace goap
{

class IState : public virtual IStringPrintable, public virtual IClonable, public virtual IHashable
{
public:
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;

    typedef std::map<std::string, float> map_string_float_type;

    typedef float value_type;
    typedef intptr_t index_type;
    typedef std::pair<IStateValue::CPtr, IStateValue::Ptr> pair_value;

    virtual void remove(const std::string &str) = 0;
    virtual void remove(const IStateValue::CPtr &key) = 0;
    virtual IState::Ptr put(const std::string &str, const std::string &value) = 0;
    virtual IState::Ptr put(const std::string &str, const IStateValue::Ptr &value) = 0;
    virtual IState::Ptr put(const std::string &str, std::initializer_list<float> list) = 0;
    virtual IState::Ptr put(const std::string &str, float number) = 0;
    virtual IState::Ptr put(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IState::Ptr add(const std::string &str, const std::string &value) = 0;
    virtual IState::Ptr add(const std::string &str, const IStateValue::Ptr &value) = 0;
    virtual IState::Ptr add(const std::string &str, std::initializer_list<float> list) = 0;
    virtual IState::Ptr add(const std::string &str, float number) = 0;
    virtual IState::Ptr add(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IState::Ptr mul(const std::string &str, const std::string &value) = 0;
    virtual IState::Ptr mul(const std::string &str, const IStateValue::Ptr &value) = 0;
    virtual IState::Ptr mul(const std::string &str, std::initializer_list<float> list) = 0;
    virtual IState::Ptr mul(const std::string &str, float number) = 0;
    virtual IState::Ptr mul(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IStateValue & atRef(const IStateValue::CPtr &key) const = 0;
    virtual IStateValue & atRef(const std::string &str) const = 0;
    const IStateValue::Ptr at(const IStateValue::CPtr &key) const { return IStateValue::Ptr(&atRef(key)); }
    const IStateValue::Ptr at(const std::string &str) const { return IStateValue::Ptr(&atRef(str)); }
    virtual pair_value at(intptr_t idx) const = 0;
    virtual intptr_t size() const = 0;
    virtual void assign(const IState::CPtr &other) = 0;
    virtual void assign(const map_string_float_type &map_string_float) = 0;
    virtual bool equals(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual void cost(float c) = 0;
    virtual IState::Ptr addCost(float c) = 0;
    virtual IState::Ptr mulCost(float c) = 0;
};

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
    std::size_t operator()(const IState::CPtr &k) const {
        return k->hash();
    }
};

template <>
struct hash<IState::Ptr>
{
    std::size_t operator()(const IState::Ptr &k) const {
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
