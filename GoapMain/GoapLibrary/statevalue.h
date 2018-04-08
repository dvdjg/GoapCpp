#ifndef STATEVALUE_H
#define STATEVALUE_H

#include <vector>
#include <memory>
#include "refcounter.h"
#include "goap/istatevalue.h"

namespace goap
{

class StateValue : public virtual IStateValue
{
    IMPLEMENT_REFCOUNTER()

protected:
    std::vector<float> data;

public:
    StateValue();
    StateValue(const StateValue & other);
    StateValue(const std::string &str);

    std::size_t size() const override;
    void resize(std::size_t len) override;
    float at(float idx) const override;
    void setAt(float idx, float value) override;
    void fromString(const std::string &str) override;
    void interpolateFrom(const IStateValue *other) override;
    float cosineDistance(const IStateValue *other) const override;
    std::string toString() const override;
    std::size_t hash() const override;

    IClonable *clone() const override;
};

typedef boost::intrusive_ptr<StateValue> PtrStateValue;
typedef boost::intrusive_ptr<const StateValue> CPtrStateValue;

}
#endif // STATEVALUE_H
