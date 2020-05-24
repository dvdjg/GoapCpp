#ifndef GOAP_IPLANNER_H
#define GOAP_IPLANNER_H

#include <list>
#include "goap/istringvalue.h"
#include "goap/iplanningstatemeter.h"
#include "iplanningaction.h"

namespace goap
{

using namespace std;

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
    virtual list<IPlanningAction::CPtr> &makePlan(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            list<IPlanningAction::CPtr> &actionsArray,
            list<IState::CPtr>* pStates = nullptr) = 0;

    /**
     * A version of makePlan() with the help of cached plans.
     * @note The found cached plan is not warranted to use the supplied planningStateMeter.
     */
    virtual list<IPlanningAction::CPtr> &makePlanCached(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            list<IPlanningAction::CPtr> &actionsArray,
            list<IState::CPtr> *pStates = nullptr) = 0;
};


}

#endif // GOAP_IPLANNER_H
