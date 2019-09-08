#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include "refcounter.h"
#include "goap/istate.h"

namespace goap
{

class State : public virtual IState
{
    IMPLEMENT_REFCOUNTER()

protected:
    std::unordered_map<IStateValue::CPtr, IStateValue::Ptr> data;
    float coste = 0;

public:
    State();
    State(const State &other);
    State(const IState::CPtr &other);
    //static std::string g_strStateValueRegistration;

    void remove(const IStateValue::CPtr &key) override;
    IStateValue::Ptr at(const IStateValue::CPtr &key) const override;
    void setAt(const IStateValue::CPtr &key, const IStateValue::Ptr &value) override;
    size_t size() const override;
    void remove(const std::string &str) override;
    void setAt(const std::string &str, const IStateValue::Ptr &value) override;
    IStateValue::Ptr at(const std::string &str) const override;
    bool equal(const IState::CPtr &other) const override;
    float cost() const override;
    void setCost(float c) override;
    void assign(const IState::CPtr &other) override;

    PairIStateValue at(intptr_t idx) const override;

    // IClonable interface
public:
    IClonable::Ptr clone() const override;

    // IStringValue interface
public:
    void fromString(const std::string &str) override;
    std::string toDebugString() const override;
    std::string toString() const override;
};

}

#endif // STATE_H
