#include "goaplibrary.h"
#include "reuseobjectpool.h"
#include "comparerstatemeter.h"
#include "functionstatemeter.h"
#include "planningaction.h"
#include "planner.h"
#include "path.h"
#include "scopetime.h"

namespace goap
{
const std::string NUMERICSTATECOMPARER              = STR_GOAP_NUMERICSTATECOMPARER;
const std::string EXACTSTATEMETER                   = STR_GOAP_EXACTSTATECOMPARER;
const std::string NUMERICSTATECOMPARER_SINGLETON    = STR_GOAP_NUMERICSTATECOMPARER_SINGLETON;
const std::string EXACTSTATEMETER_SINGLETON         = STR_GOAP_EXACTSTATECOMPARER_SINGLETON;

Goap::Goap()
{
}

IPlanningAction::Ptr Goap::newPlanningAction(
        const char *name,
        IPlanningAction::validator_function_type validator_,
        IPlanningAction::executor_function_type executor_) {
    auto ret = RecyclableWrapper<PlanningAction>::createFromPoolRaw();
    ret->setName(NewPtr<IStringValue>()->assign(name));
    ret->setValidator(validator_);
    ret->setExecutor(executor_);
    return std::move(ret);
}

IPlanner::Ptr Goap::newPlanner(IPlanner::type planningMethod_, const std::list<IPlanningAction::CPtr> &planningActions_) {
    auto ret = RecyclableWrapper<Planner>::createFromPoolRaw();
    ret->planningMethod(planningMethod_);
    ret->planningActions(planningActions_);
    return ret;
}

IFunctionStateMeter::Ptr Goap::newFunctionStateMeter(IState::CPtr goalState) {
    auto ret = RecyclableWrapper<FunctionStateMeter>::createFromPoolRaw();
    ret->goalState(goalState);
    return ret;
}

IPlanningStateMeter::Ptr Goap::newComparerStateMeter(IState::CPtr goalState, IPlanningStateComparer::Ptr stateComparer) {
    auto ret = RecyclableWrapper<ComparerStateMeter>::createFromPoolRaw();
    ret->goalState(goalState);
    ret->comparer(stateComparer);
    return ret;
}

IPath::Ptr Goap::newPath(IPlanningAction::CPtr action_, IPath::Ptr parent_, float cost_) {
    auto ret = RecyclableWrapper<Path>::createFromPoolRaw();
    ret->action(action_);
    ret->parent(parent_);
    ret->setCost(cost_);
    return ret;
}

IScopeTime::Ptr Goap::newScopeTime(const char *szMessage, IScopeTime::pfn_time pfnTime, bool bOutOfScope) {
    auto ret = RecyclableWrapper<ScopeTime>::createFromPoolRaw();
    ret->setMessage(szMessage);
    ret->setPfnTime(pfnTime);
    ret->setMessageOnDelete(bOutOfScope);
    return ScopeTime::Ptr(ret);
}
}

