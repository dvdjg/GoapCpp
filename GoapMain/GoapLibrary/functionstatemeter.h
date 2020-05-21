#ifndef GOAP_FUNCTIONSTATEMETER_H
#define GOAP_FUNCTIONSTATEMETER_H

#include <unordered_map>
#include "goap/istate.h"
#include "goap/ifunctionstatemeter.h"
#include "planningstatemeter.h"
#include "refcounter.h"

namespace goap
{

class FunctionStateMeter : public virtual IFunctionStateMeter // public virtual PlanningStateMeter
{
    IMPLEMENT_REFCOUNTER()

public:
    typedef explicit_ptr<FunctionStateMeter>        Ptr;
    typedef explicit_ptr<const FunctionStateMeter>  CPtr;

    typedef std::unordered_map<IStateValue::CPtr, IPlanningStateMeter::CPtr>    meter_map_data_type;

protected:
    IState::CPtr _goalState;
    bool _isMonotonic = false;
    distance_function_type      _fnDistance;    // Signature: function(state:State, stateMeter:FunctionStateMeter):Number
    enough_function_type        _fnEnough;      // Signature: function(state:State, stateMeter:FunctionStateMeter):Boolean
    meter_map_data_type         _stateMeters;
    IPlanningStateMeter::Ptr    _numericStateMeter;
    IPlanningStateMeter::Ptr    _exactStateMeter;

public:
    FunctionStateMeter();

    FunctionStateMeter(IState::CPtr goalState_);

    void goalState(IState::CPtr goalState_) override;

    IState::CPtr goalState() const override;

    bool monotonic() const override;

    void monotonic(bool monotonic) override;

    void clear();

    float distance(IState::CPtr state) const override;

    bool enough(IState::CPtr state) const override;

    //void addStateMeter(const std::string& name, IPlanningStateMeter::Ptr stateMeter) override;

    void addStateMeter(const IStateValue::CNew& name, IPlanningStateMeter::Ptr stateMeter) override;

    IPlanningStateMeter::CPtr getStateMeter(IStateValue::CPtr name) override;

    IPlanningStateMeter::CPtr getStateMeter(const std::string& name) override;

    distance_function_type fnDistance() const override;

    void fnDistance(distance_function_type value) override;

    enough_function_type fnEnough() const override;

    void fnEnough(enough_function_type value) override;

    IPlanningStateMeter::Ptr numericStateMeter() const override;

    IPlanningStateMeter::Ptr exactStateMeter() const override;
};

}

#endif // GOAP_FUNCTIONSTATEMETER_H
