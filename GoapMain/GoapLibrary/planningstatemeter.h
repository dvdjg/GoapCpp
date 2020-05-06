#ifndef GOAP_PLANNINGSTATEMETER_H
#define GOAP_PLANNINGSTATEMETER_H

#include "refcounter.h"
#include "goap/istate.h"
#include "goap/iplanningstatemeter.h"

namespace goap
{
/**
* Helper class for measuring the distance from a given state to the goal state.
* This class must be overrided to implement valid measuring functions.
*/
class PlanningStateMeter : public virtual IPlanningStateMeter
{
    IMPLEMENT_REFCOUNTER()

protected:
    IState::Ptr _goalState;
    bool _isMonotonic = false;

public:
    PlanningStateMeter(const PlanningStateMeter& o) : _goalState(o._goalState), _isMonotonic(o._isMonotonic)
    {
    }

    PlanningStateMeter(IState::CPtr goalState)
    {
        _goalState = goalState;
    }

    void clear() {
        _goalState.reset();
        _isMonotonic = false;
    }

    /**
     * Returns true when the input 'state' satisfies the goal state.
     */
    bool enough(IState::CPtr state) const override
    {
        return distance(state) == 0;
    }

    /**
     * Returns the measure from the input state to the goal state
     */
    float distance(IState::CPtr state) const override
    {
        (void)state;
        return 0;
    }

    IState::CPtr goalState() const override
    {
        return _goalState;
    }

    bool monotonic() const override
    {
        return _isMonotonic;
    }

    void setMonotonic(bool monotonic)
    {
        _isMonotonic = monotonic;
    }
};

}

#endif // GOAP_PLANNINGSTATEMETER_H
