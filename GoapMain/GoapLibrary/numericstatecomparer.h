#ifndef GOAP_NUMERICSTATECOMPARER_H
#define GOAP_NUMERICSTATECOMPARER_H

#include "refcounter.h"
#include "goap/iplanningstatecomparer.h"

namespace goap
{

class NumericStateComparer : public IPlanningStateComparer
{
    IMPLEMENT_REFCOUNTER()

public:
    typedef explicit_ptr<NumericStateComparer> Ptr;
    typedef explicit_ptr<const NumericStateComparer> CPtr;

    static NumericStateComparer::Ptr singleton();

    NumericStateComparer();

    /**
     * Returns true when the state has reach the goal, so the input 'state' satisfies the goal state.
     * state2 is the goal state. Test if the common keys are equal.
     */
    bool enough(IState::CPtr state1, IState::CPtr state2) const override;

    /**
     * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
     * A percentage numeric distance is used to compute de similarities.
     * Both states must have numerical values.
     * Both have the same state if state1 has at least the same fields than state2. Anyway, s1 can have more states.
     * Only the common keys are compared.
     */
    float distance(IState::CPtr stateSrc, IState::CPtr stateDst) const override;
};

}
#endif // NUMERICSTATECOMPARER_H
