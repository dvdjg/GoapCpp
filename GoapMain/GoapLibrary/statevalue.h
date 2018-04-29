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
    StateValue(const IStateValue::CPtr &other);
    StateValue(const std::string &str);

    std::size_t size() const override;
    void resize(std::size_t len) override;
    float atF(float idx) const override;
    void setAtF(float idx, float value) override;
    float at(size_t idx) const override;
    void setAt(size_t idx, float value) override;
    void fromString(const std::string &str) override;
    void interpolateFrom(const IStateValue::CPtr &other) override;
    float cosineDistance(const IStateValue::CPtr &other) const override;
    void assign(const IStateValue::CPtr &other) override;
    std::string toString() const override;
    std::size_t hash() const override;

    IClonable::Ptr clone() const override;

};

}
#endif // STATEVALUE_H
