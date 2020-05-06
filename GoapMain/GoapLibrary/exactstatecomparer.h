#ifndef GOAP_EXACTSTATECOMPARER_H
#define GOAP_EXACTSTATECOMPARER_H

#include "refcounter.h"
#include "goap/istate.h"
#include "numericstatecomparer.h"

namespace goap
{

class ExactStateComparer : public NumericStateComparer
{
    IMPLEMENT_REFCOUNTER_PARENT(NumericStateComparer)

public:
    typedef explicit_ptr<ExactStateComparer> Ptr;
    typedef explicit_ptr<const ExactStateComparer> CPtr;

    static ExactStateComparer::Ptr singleton();

    /**
     * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
     * A percentage exact state members count is used to compute de similarities.
     * Both states can be of any type. The != operator is used to find similarities
     */
    //bool distancePure(IState::CPtr state1, IState::CPtr state2) const;

    /**
     * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
     * A percentage numeric distance is used to compute de similarities.
     * Both states must have numerical values.
     * Both have the same state if state1 has at least the same fields than state2. Anyway, state1 can have more states.
     * Only the common keys are compared.
     */
    float distance(IState::CPtr state1, IState::CPtr state2) const override;
};

}
#endif // EXACTSTATECOMPARER_H
