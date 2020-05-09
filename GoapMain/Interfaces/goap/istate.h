#ifndef GOAP_IREADSTATE_H
#define GOAP_IREADSTATE_H

#include <memory>
#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "istatevalue.h"

namespace goap
{

class IState : public IStringPrintable, public IClonable, public virtual IRefCounter
{
public:
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;

    typedef float value_type;
    typedef intptr_t index_type;
    typedef std::pair<IStateValue::CPtr, IStateValue::Ptr> pair_value;

    virtual void remove(const std::string &str) = 0;
    virtual void remove(const IStateValue::CPtr &key) = 0;
    virtual void setAt(const std::string &str, const IStateValue::Ptr &value) = 0;
    virtual void setAt(const std::string &str, std::initializer_list<float> list) = 0;
    virtual void setAt(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IStateValue::Ptr at(const IStateValue::CPtr &key) const = 0;
    virtual IStateValue::Ptr at(const std::string &str) const = 0;
    virtual pair_value at(intptr_t idx) const = 0;
    virtual intptr_t size() const = 0;
    virtual void assign(const IState::CPtr &other) = 0;
    virtual bool equal(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual void cost(float c) = 0;
};

inline bool operator ==(const IState& a, const IState& b)
{
    return a.equal(IState::CPtr(&b));
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
        return data1->equal(data2);
    }
};

}

#endif // GOAP_IREADSTATE_H
