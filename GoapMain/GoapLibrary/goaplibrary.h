#ifndef GOAP_LIBRARY_H
#define GOAP_LIBRARY_H

#include "goap/iscopetimer.h"
#include "goap/iplanningaction.h"
#include "goap/istringvalue.h"
#include "goap/iplanner.h"
#include "goap/iplanningstatecomparer.h"
#include "functionstatemeter.h"

namespace goap
{

class Goap
{

public:
    Goap();

    static IScopeTime::Ptr newScopeTime(const char *szMessage = nullptr, IScopeTime::pfn_time pfnTime = nullptr, bool bOutOfScope = true);

    static IPlanningAction::Ptr newPlanningAction(
            const char* name,
            IPlanningAction::validator_function_type validator_ = {},
            IPlanningAction::executor_function_type executor_ = {});

    static IPlanner::Ptr newPlanner(
            IPlanner::type planningMethod_,
            const std::list<IPlanningAction::CPtr> &planningActions_);

    static FunctionStateMeter::Ptr newFunctionStateMeter(IState::CPtr goalState);

    static IPlanningStateMeter::Ptr newComparerStateMeter(IState::CPtr goalState, IPlanningStateComparer::Ptr stateComparer = {});
};

}

#endif // GOAP_LIBRARY_H
