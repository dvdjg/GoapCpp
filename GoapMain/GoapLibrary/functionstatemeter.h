#ifndef GOAP_FUNCTIONSTATEMETER_H
#define GOAP_FUNCTIONSTATEMETER_H

#include <unordered_map>
#include <functional>
#include "refcounter.h"
#include "goap/istate.h"
#include "planningstatemeter.h"
#include "comparerstatemeter.h"
#include "exactstatecomparer.h"
#include "newptr.h"

namespace goap
{

class FunctionStateMeter : public virtual PlanningStateMeter
{
    IMPLEMENT_REFCOUNTER_PARENT(PlanningStateMeter)

public:
    typedef explicit_ptr<FunctionStateMeter> Ptr;
    typedef explicit_ptr<const FunctionStateMeter> CPtr;

    typedef std::function<float(IState::CPtr, FunctionStateMeter::CPtr)> distance_function_type;
    typedef std::function<bool(IState::CPtr, FunctionStateMeter::CPtr)> enough_function_type;
    typedef std::unordered_map<IStateValue::CPtr, IPlanningStateMeter::CPtr> meter_map_data_type;

protected:
    distance_function_type _fnDistance; // Signature: function(state:State, stateMeter:FunctionStateMeter):Number
    enough_function_type _fnEnough; // Signature: function(state:State, stateMeter:FunctionStateMeter):Boolean
    meter_map_data_type _stateMeters;
    ComparerStateMeter::Ptr _numericStateMeter;
    ComparerStateMeter::Ptr _exactStateMeter;

public:
    FunctionStateMeter(IState::CPtr goalState) : PlanningStateMeter(goalState) {
        _numericStateMeter = new ComparerStateMeter(goalState, NumericStateComparer::singleton());
        addStateMeter("numeric", _numericStateMeter);
        _exactStateMeter = new ComparerStateMeter(goalState, ExactStateComparer::singleton());
        _exactStateMeter->setMonotonic(true);
        addStateMeter("exact", _exactStateMeter);

        _fnDistance = [](IState::CPtr state, FunctionStateMeter::CPtr stateMeter) {
            return stateMeter->numericStateMeter()->distance(state);
        };
        _fnEnough = [](IState::CPtr state, FunctionStateMeter::CPtr stateMeter) {
            return stateMeter->numericStateMeter()->enough(state);
        };
    }


    float distance(IState::CPtr state) const override {
        return _fnDistance(state, FunctionStateMeter::CPtr(this));
    }

    bool enough(IState::CPtr state) const override {
        return _fnEnough(state, this);
    }

    void addStateMeter(const std::string& name, IPlanningStateMeter::Ptr stateMeter) {
        addStateMeter(NewPtr<IStateValue>({}, name), stateMeter);
    }

    void addStateMeter(IStateValue::CPtr name, IPlanningStateMeter::Ptr stateMeter) {
        _stateMeters[name] = stateMeter;
    }

    IPlanningStateMeter::CPtr getStateMeter(IStateValue::CPtr name) {
        return _stateMeters[name];
    }

    IPlanningStateMeter::CPtr getStateMeter(const std::string& name) {
        return getStateMeter(NewPtr<IStateValue>({}, name));
    }

    distance_function_type fnDistance() const {
        return _fnDistance;
    }

    void fnDistance(distance_function_type value) {
        _fnDistance = value;
    }

    enough_function_type fnEnough() const {
        return _fnEnough;
    }

    void fnEnough(enough_function_type value) {
        _fnEnough = value;
    }

    ComparerStateMeter::Ptr numericStateMeter() const
    {
        return _numericStateMeter;
    }

    ComparerStateMeter::Ptr exactStateMeter() const
    {
        return _exactStateMeter;
    }
};

}

#endif // GOAP_FUNCTIONSTATEMETER_H
