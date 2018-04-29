#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include "refcounter.h"
#include "goap/ireadstate.h"

namespace goap
{

class State : public virtual IState
{
    IMPLEMENT_REFCOUNTER()

protected:
    std::unordered_map<IStateValue::Ptr, IStateValue::Ptr> data;
    float coste = 0;

public:
    State();

    void remove(const IStateValue::Ptr &key) override;
    IStateValue::Ptr at(const IStateValue::Ptr &key) const override;
    void setAt(const IStateValue::Ptr &key, const IStateValue::Ptr &value) override;
    size_t size() const override;
    void remove(const std::string &str) override;
    void setAt(const std::string &str, const IStateValue::Ptr &value) override;
    IStateValue::Ptr at(const std::string &str) const override;
    bool equals(const IState::CPtr &other) const override;
    float cost() const override;
    void setCost(float c) override;

    std::pair<IStateValue::Ptr, IStateValue::Ptr> at(intptr_t idx) const override;
};

}

#endif // STATE_H
