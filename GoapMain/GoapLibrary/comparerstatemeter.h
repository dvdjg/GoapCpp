#ifndef GOAP_COMPARERSTATEMETER_H
#define GOAP_COMPARERSTATEMETER_H

#include "refcounter.h"
#include "goap/istate.h"
#include "planningstatemeter.h"
#include "numericstatecomparer.h"
#include "newptr.h"

namespace goap
{
/**
* Helper class for measuring the distance from a given state to the goal state.
* This class must be overrided to implement valid measuring functions.
*/
class ComparerStateMeter : public PlanningStateMeter
{
    IMPLEMENT_REFCOUNTER_PARENT(PlanningStateMeter)

protected:
    IPlanningStateComparer::Ptr _comparer;

public:
    typedef explicit_ptr<ComparerStateMeter> Ptr;
    typedef explicit_ptr<const ComparerStateMeter> CPtr;

    ComparerStateMeter() {
    }

    ComparerStateMeter(IState::CPtr goalState, IPlanningStateComparer::Ptr stateComparer = {}) : PlanningStateMeter(goalState) {
        comparer(stateComparer);
    }

    void comparer(IPlanningStateComparer::Ptr stateComparer = {}) {
        _comparer = stateComparer ? stateComparer : NumericStateComparer::singleton();
    }

    float distance(IState::CPtr state) const override
    {
        return _comparer->distance(state, _goalState);
    }

    bool enough(IState::CPtr state) const override
    {
        return _comparer->enough(state, _goalState);
    }

    IPlanningStateComparer::Ptr comparer()
    {
        return _comparer;
    }

};

}

#endif // COMPARERSTATEMETER_H
