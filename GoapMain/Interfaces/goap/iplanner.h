#ifndef GOAP_IPLANNER_H
#define GOAP_IPLANNER_H

#include <list>
#include "common/irefcounter.h"
#include "iplanningaction.h"
#include "goap/iplanningstatemeter.h"

namespace goap
{

class IPlanner : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IPlanner> Ptr;
    typedef explicit_ptr<const IPlanner> CPtr;

    virtual std::list<IPlanningAction::CPtr> makePlan(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            std::list<IPlanningAction::CPtr> actionsArray = {},
            std::list<IState::CPtr> states = {}) = 0;
};

}

#endif // GOAP_IPLANNER_H
