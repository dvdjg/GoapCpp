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

public:
    typedef std::unordered_map<IStateValue::CPtr, IStateValue::Ptr> data_type;
    data_type data;
protected:
    float coste = 0;

public:
    State();
    State(const State &other);
    State(const IState::CPtr &other);    
    State(std::initializer_list<data_type::value_type> list);

    void remove(const IStateValue::CPtr &key) override;
    IStateValue::Ptr at(const IStateValue::CPtr &key) const override;
    IStateValue::Ptr at(const std::string &str) const override;
    void setAt(const IStateValue::CPtr &key, const IStateValue::Ptr &value) override;
    void setAt(const std::string &str, const IStateValue::Ptr &value) override;
    void setAt(const std::string &str, std::initializer_list<float> list) override;
    intptr_t size() const override;
    void remove(const std::string &str) override;
    bool equal(const IState::CPtr &other) const override;
    float cost() const override;
    void setCost(float c) override;
    void assign(const IState::CPtr &other) override;
    void assign(const State &other);

    pair_value at(intptr_t idx) const override;

    // IClonable interface
public:
    IClonable::Ptr clone() const override;

    // IStringPrintable interface
public:
    std::string toDebugString() const override;
    std::string toString() const override;
};

}

#endif // STATE_H
