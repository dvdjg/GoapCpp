#ifndef STATEVALUE_H
#define STATEVALUE_H

#include <vector>
#include <memory>
#include "refcounter.h"
#include "goap/istatevalue.h"

namespace goap
{

class StateValue : public IStateValue
{
    IMPLEMENT_REFCOUNTER()

protected:
    std::vector<float> data;

public:    
    typedef explicit_ptr<StateValue> Ptr;
    typedef explicit_ptr<const StateValue> CPtr;

    StateValue();
    StateValue(const StateValue & other);
    StateValue(const std::string &str);

    std::size_t size() const override;
    void resize(std::size_t len) override;
    float at(float idx) const override;
    void setAt(float idx, float value) override;
    void fromString(const std::string &str) override;
    void interpolateFrom(const IStateValue::CPtr &other) override;
    float cosineDistance(const IStateValue::CPtr &other) const override;
    std::string toString() const override;
    std::size_t hash() const override;

    IClonable::Ptr clone() const override;
};

}
#endif // STATEVALUE_H
