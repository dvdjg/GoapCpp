#ifndef GOAP_LIBRARY_H
#define GOAP_LIBRARY_H

#include "goap/iscopetimer.h"
#include "goap/iplanningaction.h"
#include "goap/istringvalue.h"
#include "goap/iplanner.h"
#include "goap/iplanningstatecomparer.h"
#include "goap/ipath.h"
#include "functionstatemeter.h"

namespace goap
{

extern const std::string NUMERICSTATECOMPARER;
extern const std::string EXACTSTATEMETER;
extern const std::string NUMERICSTATECOMPARER_SINGLETON;
extern const std::string EXACTSTATEMETER_SINGLETON;

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
    static IPath::Ptr newPath(IPlanningAction::CPtr action_, IPath::Ptr parent_ = {}, float cost_ = 0);
};

}

#endif // GOAP_LIBRARY_H
