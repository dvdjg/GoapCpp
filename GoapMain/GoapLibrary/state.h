#ifndef STATE_H
#define STATE_H

#include "ireadstate.h"

class State : public IState
{
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
};

#endif // STATE_H
