#ifndef STATEVALUE_H
#define STATEVALUE_H
#include <vector>
#include <memory>
#include "istatevalue.h"

class StateValue : public IStateValue, public std::enable_shared_from_this<StateValue>
{
protected:
    std::vector<half> data;

public:
    StateValue();

    // IStateValue interface
public:
    std::size_t length() const override;
    float at(float idx) const override;
    void setAt(float idx, float value) override;
    void fromString(const std::u16string &str) override;
    std::u16string toString() const override;
    std::size_t hash() const override;
};

#endif // STATEVALUE_H
