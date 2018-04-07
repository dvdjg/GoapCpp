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
    std::unordered_map<PtrIStateValue, PtrIStateValue> data;
    float coste = 0;

public:
    State();

    void remove(const PtrIStateValue &key) override;
    PtrIStateValue at(const PtrIStateValue &key) const override;
    void setAt(const PtrIStateValue &key, const PtrIStateValue &value) override;
    size_t size() const override;
    void remove(const std::string &str) override;
    void setAt(const std::string &str, const PtrIStateValue &value) override;
    PtrIStateValue at(const std::string &str) const override;
    bool equals(const IState *other) const override;
    float cost() const override;
    void setCost(float c) override;

    std::pair<PtrIStateValue, PtrIStateValue> at(intptr_t idx) const override;
};

}

#endif // STATE_H
