#ifndef GOAP_IPLANNER_H
#define GOAP_IPLANNER_H

#include <list>
#include "common/irefcounter.h"
#include "iplanningaction.h"
#include "goap/iplanningstatemeter.h"

namespace goap
{

class IPlanner : public IStringPrintable
{
public:
    typedef explicit_ptr<IPlanner> Ptr;
    typedef explicit_ptr<const IPlanner> CPtr;

    enum type {
        BreadthFirst = 1, // Queue: FIFO
        DepthFirst = 2    // Stack: LIFO
    };

    virtual IPlanner::type planningMethod() = 0;
    virtual void planningMethod(IPlanner::type method) = 0;

    /**
     * Returns the plan: A Vector of IPlanningAction.
     * If no plan is found a null is returned.
     */
    virtual std::list<IPlanningAction::CPtr> &makePlan(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            std::list<IPlanningAction::CPtr> &actionsArray,
            std::list<IState::CPtr>* pStates = nullptr) = 0;

    /**
     * A version of makePlan() with the help of cached plans.
     * @note The found cached plan is not warranted to use the supplied planningStateMeter.
     */
    virtual std::list<IPlanningAction::CPtr> &makePlanCached(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            std::list<IPlanningAction::CPtr> &actionsArray,
            std::list<IState::CPtr> *pStates = nullptr) = 0;
};

}

#endif // GOAP_IPLANNER_H
