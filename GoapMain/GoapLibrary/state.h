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
    std::unordered_map<PtrIValue, PtrIValue> data;
    float coste = 0;

public:
    State();

    void remove(const PtrIValue &key) override;
    PtrIValue at(const PtrIValue &key) const override;
    void setAt(const PtrIValue &key, const PtrIValue &value) override;
    size_t size() const override;
    void remove(const std::string &str) override;
    void setAt(const std::string &str, const PtrIValue &value) override;
    PtrIValue at(const std::string &str) const override;
    bool equals(const IState *other) const override;
    float cost() const override;
    void setCost(float c) override;

    std::pair<PtrIValue, PtrIValue> at(intptr_t idx) const override;
};

}

#endif // STATE_H
