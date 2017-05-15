#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include "ireadstate.h"

namespace goap
{

class State : public IState
{
protected:
    std::unordered_map<ptr_value, ptr_value> data;
    float coste = 0;

public:
    State();

    void remove(const ptr_value &key) override;
    ptr_value at(const ptr_value &key) const override;
    void setAt(const ptr_value &key, const ptr_value &value) override;
    size_t size() const override;
    void remove(const std::u16string &str) override;
    void setAt(const std::u16string &str, const ptr_value &value) override;
    ptr_value at(const std::u16string &str) const override;
    bool equals(const IState *other) const override;
    float cost() const override;
    void setCost(float c) override;

    std::pair<ptr_value, ptr_value> at(ssize_t idx) const override;
};

}

#endif // STATE_H
