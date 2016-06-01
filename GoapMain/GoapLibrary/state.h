#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include "ireadstate.h"

class State : public IState
{
    std::unordered_map<ptr_value, ptr_value> data;
public:
    State();

    // IState interface
public:
    void remove(const ptr_value &key) const override;
    ptr_value at(const ptr_value &key) const override;
    ptr_value &setAt(const ptr_value &key, const ptr_value &value) override;
    size_t size() const override;
    bool equals(const IState *other) const override;
    float cost() const override;
    float &setCost(float c) const override;

    std::pair<ptr_value, ptr_value> at(ssize_t idx) const override;
};

#endif // STATE_H
