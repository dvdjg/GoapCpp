#ifndef STATEVALUE_H
#define STATEVALUE_H
#include <vector>
#include <memory>
#include "istatevalue.h"

namespace goap
{
class StateValue : public IStateValue,
        public std::enable_shared_from_this<StateValue>
{
protected:
    std::vector<half> data;

public:
    typedef std::enable_shared_from_this<StateValue> parent;
    StateValue();
    StateValue(const StateValue & other);
    StateValue(const std::u16string &str);

    std::size_t size() const override;
    void resize(std::size_t len) override;
    float at(float idx) const override;
    void setAt(float idx, float value) override;
    void fromString(const std::u16string &str) override;
    void interpolateFrom(const IStateValue * other) override;
    float cosineDistance(const IStateValue * other) const override;
    std::u16string toString() const override;
    std::size_t hash() const override;

    ptr_value clone() const override;
};
}
#endif // STATEVALUE_H
