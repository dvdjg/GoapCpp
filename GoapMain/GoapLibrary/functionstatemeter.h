#ifndef GOAP_FUNCTIONSTATEMETER_H
#define GOAP_FUNCTIONSTATEMETER_H

#include <unordered_map>
#include "goap/istate.h"
#include "planningstatemeter.h"
#include "refcounter.h"

namespace goap
{

class FunctionStateMeter : public virtual PlanningStateMeter // , public virtual IFunctionStateMeter
{
    IMPLEMENT_REFCOUNTER_PARENT(PlanningStateMeter)

public:
    typedef explicit_ptr<FunctionStateMeter>        Ptr;
    typedef explicit_ptr<const FunctionStateMeter>  CPtr;

    typedef std::unordered_map<IStateValue::CPtr, IPlanningStateMeter::CPtr>    meter_map_data_type;
    typedef std::function<float(IState::CPtr, FunctionStateMeter::CPtr)>        distance_function_type;
    typedef std::function<bool(IState::CPtr, FunctionStateMeter::CPtr)>         enough_function_type;

protected:
    distance_function_type      _fnDistance;    // Signature: function(state:State, stateMeter:FunctionStateMeter):Number
    enough_function_type        _fnEnough;      // Signature: function(state:State, stateMeter:FunctionStateMeter):Boolean
    meter_map_data_type         _stateMeters;
    IPlanningStateMeter::Ptr    _numericStateMeter;
    IPlanningStateMeter::Ptr    _exactStateMeter;

public:
    FunctionStateMeter();

    FunctionStateMeter(IState::CPtr goalState_);

    void goalState(IState::CPtr goalState_) override;

    void clear() override;

    float distance(IState::CPtr state) const override;

    bool enough(IState::CPtr state) const override;

    void addStateMeter(const std::string& name, IPlanningStateMeter::Ptr stateMeter);

    void addStateMeter(IStateValue::CPtr name, IPlanningStateMeter::Ptr stateMeter);

    IPlanningStateMeter::CPtr getStateMeter(IStateValue::CPtr name);

    IPlanningStateMeter::CPtr getStateMeter(const std::string& name);

    distance_function_type fnDistance() const;

    void fnDistance(distance_function_type value);

    enough_function_type fnEnough() const;

    void fnEnough(enough_function_type value);

    IPlanningStateMeter::Ptr numericStateMeter() const;

    IPlanningStateMeter::Ptr exactStateMeter() const;
};

}

#endif // GOAP_FUNCTIONSTATEMETER_H
