#ifndef GOAP_IPLANNINGSTATEMETER_H
#define GOAP_IPLANNINGSTATEMETER_H

#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "istate.h"

#define STR_GOAP_COMPARERSTATEMETER "ComparerStateMeter"
#define STR_GOAP_FUNCTIONSTATEMETER "FunctionStateMeter"

namespace goap
{

class IPlanningStateMeter : public IRefCounter
{
public:
    typedef explicit_ptr<IPlanningStateMeter> Ptr;
    typedef explicit_ptr<const IPlanningStateMeter> CPtr;

    /**
     * Returns true when the state has reach the goal, so the input 'state' satisfies the goal state.
     */
    virtual bool enough(IState::CPtr state) const = 0;

    /**
     * Returns the measure from the input state to the goal state.
     * It will return lower numbers when it deems that the goal is close so the planner can direct the search.
     */
    virtual float distance(IState::CPtr state) const = 0;

    /**
     * If true, the distance functions gives hints for Landmarkable search:
     * The distance supplied gives strong suggestions. It really means the real distance to the result.
     *
     * If the distante function gives only vage hints (the typical case), monotonic must be false.
     */
    virtual bool monotonic() const = 0;

    virtual void setMonotonic(bool monotonic) = 0;

    virtual IState::CPtr goalState() const = 0;

    virtual void goalState(IState::CPtr goalState) = 0;
};

}

#endif // GOAP_IPLANNINGSTATEMETER_H
