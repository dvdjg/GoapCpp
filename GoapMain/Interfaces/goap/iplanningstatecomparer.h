#ifndef GOAP_IPLANNINGSTATECOMPARER_H
#define GOAP_IPLANNINGSTATECOMPARER_H

#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "istate.h"

namespace goap
{

class IPlanningStateComparer : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IPlanningStateComparer> Ptr;
    typedef explicit_ptr<const IPlanningStateComparer> CPtr;

    /**
     * Returns true when state1 is similar enough to state2 for stopping the search.
     */
    virtual bool enough(IState::CPtr state1, IState::CPtr state2) const = 0;

    /**
     * Returns the measure from the state1 to state2.
     */
    virtual float distance(IState::CPtr state1, IState::CPtr state2) const = 0;

};

}

#endif // GOAP_IPLANNINGSTATECOMPARER_H
