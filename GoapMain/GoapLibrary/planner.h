#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include <list>
#include "goap/iplanner.h"

namespace goap
{

class Planner : public IPlanner
{
public:
    typedef explicit_ptr<Planner> Ptr;
    typedef explicit_ptr<const Planner> CPtr;

    virtual std::list<IPlanningAction::CPtr> makePlan(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            std::list<IPlanningAction::CPtr> actionsArray = {},
            std::list<IState::CPtr> states = {});
};

}

#endif // GOAP_PLANNER_H
