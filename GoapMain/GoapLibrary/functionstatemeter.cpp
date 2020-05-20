#include "functionstatemeter.h"
#include "comparerstatemeter.h"
#include "exactstatecomparer.h"
#include "goaplibrary.h"
#include "newptr.h"

namespace goap
{

FunctionStateMeter::FunctionStateMeter() {

    _fnDistance = [](IState::CPtr state, IFunctionStateMeter::CPtr stateMeter) {
        return stateMeter->numericStateMeter()->distance(state);
    };
    _fnEnough = [](IState::CPtr state, IFunctionStateMeter::CPtr stateMeter) {
        return stateMeter->numericStateMeter()->enough(state);
    };
}

FunctionStateMeter::FunctionStateMeter(IState::CPtr goalState_) : FunctionStateMeter() {
    goalState(goalState_);
}

void FunctionStateMeter::goalState(IState::CPtr goalState_) {
    _goalState = goalState_;
    _numericStateMeter = Goap::newComparerStateMeter(goalState_, NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON));
    addStateMeter("numeric", _numericStateMeter);
    _exactStateMeter = Goap::newComparerStateMeter(goalState_, NewPtr<IPlanningStateComparer>(EXACTSTATEMETER_SINGLETON));
    _exactStateMeter->monotonic(true);
    addStateMeter("exact", _exactStateMeter);
}

IState::CPtr FunctionStateMeter::goalState() const {
    return _goalState;
}

bool FunctionStateMeter::monotonic() const {
    return _isMonotonic;
}

void FunctionStateMeter::monotonic(bool monotonic) {
    _isMonotonic = monotonic;
}

void FunctionStateMeter::clear() {
    _numericStateMeter.reset();
    _exactStateMeter.reset();
    _stateMeters.clear();
    _goalState.reset();
    _isMonotonic = false;
}

float FunctionStateMeter::distance(IState::CPtr state) const {
    return _fnDistance(state, FunctionStateMeter::CPtr(this));
}

bool FunctionStateMeter::enough(IState::CPtr state) const {
    return _fnEnough(state, this);
}

void FunctionStateMeter::addStateMeter(const string &name, IPlanningStateMeter::Ptr stateMeter) {
    addStateMeter(NewPtr<IStateValue>({}, name), stateMeter);
}

void FunctionStateMeter::addStateMeter(IStateValue::CPtr name, IPlanningStateMeter::Ptr stateMeter) {
    _stateMeters[name] = stateMeter;
}

IPlanningStateMeter::CPtr FunctionStateMeter::getStateMeter(IStateValue::CPtr name) {
    return _stateMeters[name];
}

IPlanningStateMeter::CPtr FunctionStateMeter::getStateMeter(const string &name) {
    return getStateMeter(NewPtr<IStateValue>({}, name));
}

FunctionStateMeter::distance_function_type FunctionStateMeter::fnDistance() const {
    return _fnDistance;
}

void FunctionStateMeter::fnDistance(FunctionStateMeter::distance_function_type value) {
    _fnDistance = value;
}

FunctionStateMeter::enough_function_type FunctionStateMeter::fnEnough() const {
    return _fnEnough;
}

void FunctionStateMeter::fnEnough(FunctionStateMeter::enough_function_type value) {
    _fnEnough = value;
}

IPlanningStateMeter::Ptr FunctionStateMeter::numericStateMeter() const {
    return _numericStateMeter;
}

IPlanningStateMeter::Ptr FunctionStateMeter::exactStateMeter() const {
    return _exactStateMeter;
}


}
