#ifndef IREADSTATE_H
#define IREADSTATE_H

#include <memory>
#include "common/irefcounter.h"
#include "istatevalue.h"

namespace goap
{

class IState : public IRefCounter
{
public:
    typedef explicit_ptr<IState> Ptr;
    typedef explicit_ptr<const IState> CPtr;

    virtual void remove(const IStateValue::Ptr &key) = 0;
    virtual void setAt(const IStateValue::Ptr &key, const IStateValue::Ptr &value) = 0;
    virtual IStateValue::Ptr at(const IStateValue::Ptr &key) const = 0;
    virtual void remove(const std::string &str) = 0;
    virtual void setAt(const std::string &str, const IStateValue::Ptr &value) = 0;
    virtual IStateValue::Ptr at(const std::string &str) const = 0;
    virtual std::pair<IStateValue::Ptr, IStateValue::Ptr> at(intptr_t idx) const = 0;
    virtual size_t size() const = 0;
    virtual bool equals(const IState::CPtr &other) const = 0;
    virtual float cost() const = 0;
    virtual void setCost(float c) = 0;
};

}

#endif // IREADSTATE_H
