#ifndef IREADSTATE_H
#define IREADSTATE_H

#include <memory>
#include "common/irefcounter.h"
#include "istatevalue.h"

namespace goap
{

class IState : public IStringValue, public IClonable, public virtual IRefCounter
{
public:
    typedef float value_type;
    typedef intptr_t index_type;
    typedef std::size_t size_type;
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;
    typedef std::pair<IStateValue::CPtr, IStateValue::Ptr> PairIStateValue;

    virtual void remove(const std::string &str) = 0;
    virtual void remove(const IStateValue::CPtr &key) = 0;
    virtual void setAt(const std::string &str, const IStateValue::Ptr &value) = 0;
    virtual void setAt(const IStateValue::CPtr &key, const IStateValue::Ptr &value) = 0;
    virtual IStateValue::Ptr at(const IStateValue::CPtr &key) const = 0;
    virtual IStateValue::Ptr at(const std::string &str) const = 0;
    virtual PairIStateValue at(intptr_t idx) const = 0;
    virtual size_t size() const = 0;
    virtual void assign(const IState::CPtr &other) = 0;
    virtual bool equal(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual void setCost(float c) = 0;
};

inline bool operator ==(const IState& a, const IState& b)
{
    return a.equal(IState::CPtr(&b));
}

inline bool operator !=(const IState& a, const IState& b)
{
    return !a.equal(IState::CPtr(&b));
}
}

#endif // IREADSTATE_H
