#ifndef GOAPLIBINSCRIBE_CPP
#define GOAPLIBINSCRIBE_CPP

#include <string>
#include "factory.h"
#include "reuseobjectpool.h"
#include "common/iroot.h"
#include "basicsinkcollection.h"
#include "basicostreamsink.h"
#include "statevalue.h"
#include "state.h"
#include "planningaction.h"
#include "scopetimeostream.h"
#include "path.h"
#include "planner.h"
#include "prioritizedqueue.h"
#include "prioritizedstack.h"
#include "numericstatecomparer.h"
#include "exactstatecomparer.h"
#include "comparerstatemeter.h"
#include "functionstatemeter.h"
#include "statesplan.h"

#include "goaplibinscribe.h"

using namespace goap;

int goapLibInscribeExplicit(Factory<IRoot> & factory = Factory<IRoot>::singleton(), const std::string &discr = {}) {
    int ret = 0;
    factory.inscribe<FactoryType::Default, IBasicSinkCollection, BasicSinkCollection>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSinkCollection, BasicSinkCollection, const std::string &, IBasicSink::Ptr>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSinkCollection, BasicSinkCollection, const std::string &, std::ostream &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicSinkCollection>(discr+"Collection");
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicSinkCollection, const std::string &, IBasicSink::Ptr>(discr+"Collection");
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicSinkCollection, const std::string &, std::ostream &>(discr+"Collection");
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicOstreamSink>(discr);
    ++ret;

    factory.inscribe<FactoryType::Default, IState>([](){ return RecyclableWrapper<State>::createFromPoolRaw(); }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IState>([](const IState::CPtr &cptr){
        auto ptr = RecyclableWrapper<State>::createFromPoolRaw();
        ptr->assign(cptr);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IState>([](const State &cptr){
        auto ptr = RecyclableWrapper<State>::createFromPoolRaw();
        ptr->assign(cptr);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IState>([](const std::initializer_list<State::data_type::value_type> &list){
        auto ptr = RecyclableWrapper<State>::createFromPoolRaw();
        ptr->assign(list);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IState>([](std::initializer_list<State::data_type::value_type> const list){
        auto ptr = RecyclableWrapper<State>::createFromPoolRaw();
        ptr->assign(list);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IState>([](std::initializer_list<State::data_type::value_type> &list){
        auto ptr = RecyclableWrapper<State>::createFromPoolRaw();
        ptr->assign(list);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IState>([](IState::map_value2value_type &map_string_float){
        auto ptr = RecyclableWrapper<State>::createFromPoolRaw();
        ptr->assign(map_string_float);
        return ptr;
    }, discr);
    ++ret;

    factory.inscribe<FactoryType::Default, IStateValue>([](){
        return RecyclableWrapper<StateValue>::createFromPoolRaw();
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const StateValue &cptr){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(cptr);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const IStateValue::CPtr &cptr){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(cptr);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const std::string &str){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(str);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const char *sz){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(sz);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const char *&sz){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(sz);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const std::initializer_list<float> &list){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(list);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](std::initializer_list<float> const list){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(list);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](std::initializer_list<float> &list){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(list);
        return ptr;
    }, discr);
    ++ret;

    factory.inscribe<FactoryType::Default, IStringValue>([](){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanningStateMeter>([](IState::CPtr goalState, IPlanningStateComparer::Ptr stateComparer = {}) {
        auto ret = RecyclableWrapper<ComparerStateMeter>::createFromPoolRaw();
        ret->goalState(goalState);
        ret->comparer(stateComparer);
        return ret;
    }, discr+STR_GOAP_COMPARERSTATEMETER);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanningStateMeter>([](IState::CPtr goalState) {
        auto ret = RecyclableWrapper<FunctionStateMeter>::createFromPoolRaw();
        ret->goalState(goalState);
        return ret;
    }, discr+STR_GOAP_FUNCTIONSTATEMETER);
    ++ret;
    factory.inscribe<FactoryType::Default, IFunctionStateMeter>([](IState::CPtr goalState) {
        auto ret = RecyclableWrapper<FunctionStateMeter>::createFromPoolRaw();
        ret->goalState(goalState);
        return ret;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanningStateComparer, NumericStateComparer>(discr+STR_GOAP_NUMERICSTATECOMPARER);
    ++ret;
    factory.inscribe<FactoryType::Singleton, IPlanningStateComparer, NumericStateComparer>(discr+STR_GOAP_NUMERICSTATECOMPARER_SINGLETON);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanningStateComparer, ExactStateComparer>(discr+STR_GOAP_EXACTSTATEMETER);
    ++ret;
    factory.inscribe<FactoryType::Singleton, IPlanningStateComparer, ExactStateComparer>(discr+STR_GOAP_EXACTSTATEMETER_SINGLETON);
    ++ret;
    factory.inscribe<FactoryType::Default, IPrioritized, PrioritizedQueue>(discr+STR_GOAP_PRIORITIZED_QUEUE);
    ++ret;
    factory.inscribe<FactoryType::Default, IPrioritized, PrioritizedStack>(discr+STR_GOAP_PRIORITIZED_STACK);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanningStateMeter, PlanningStateMeter>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanner, Planner>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanner>([](IPlanner::type planningMethod_, const std::list<IPlanningAction::CPtr> &planningActions_) {
        auto ret = RecyclableWrapper<Planner>::createFromPoolRaw();
        ret->planningMethod(planningMethod_);
        ret->planningActions(planningActions_);
        return ret;
    }, discr);
    ++ret;

    factory.inscribe<FactoryType::Default, IStateValue, StateValue>(discr+"Default");
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const StateValue &>(discr+"Default");
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const IStateValue::CPtr &>(discr+"Default");
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const std::string &>(discr+"Default");
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const char *>(discr+"Default");
    ++ret;
    factory.inscribe<FactoryType::Default, IScopeTime>([](const char *szMessage = nullptr, bool bOutOfScope = true) {
        auto ret = RecyclableWrapper<ScopeTimeOstream<std::cerr>>::createFromPoolRaw();
        ret->setMessage(szMessage);
        ret->setMessageOnDelete(bOutOfScope);
        return ret;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IScopeTime>([](const char *szMessage = nullptr, bool bOutOfScope = true) {
        auto ret = RecyclableWrapper<ScopeTimeOstream<std::cout>>::createFromPoolRaw();
        ret->setMessage(szMessage);
        ret->setMessageOnDelete(bOutOfScope);
        return ret;
    }, discr+"Cout");
    ++ret;
    factory.inscribe<FactoryType::Default, IScopeTime>([](const char *szMessage = nullptr, IScopeTime::pfn_time pfnTime = nullptr, bool bOutOfScope = true) {
        auto ret = RecyclableWrapper<ScopeTime>::createFromPoolRaw();
        ret->setMessage(szMessage);
        ret->setPfnTime(pfnTime);
        ret->setMessageOnDelete(bOutOfScope);
        return ret;
    }, discr);
    ++ret;

    factory.inscribe<FactoryType::Default, IPlanningAction, PlanningAction>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IPlanningAction>([](IStringValue::CPtr name, IPlanningAction::validator_function_type validator_ = {}, PlanningAction::executor_function_type executor_ = {}) {
        auto ret = RecyclableWrapper<PlanningAction>::createFromPoolRaw();
        ret->setName(name);
        ret->setValidator(validator_);
        ret->setExecutor(executor_);
        return ret;
    }, discr);
    ++ret;

    factory.inscribe<FactoryType::Default, IPath>([](IPlanningAction::CPtr action_, IPath::Ptr parent_ = {}, float cost_ = 0) {
        auto ret = RecyclableWrapper<Path>::createFromPoolRaw();
        ret->action(action_);
        ret->parent(parent_);
        ret->setCost(cost_);
        return ret;
    }, discr);
    ++ret;

    factory.inscribe<FactoryType::Default, StatesPlan>([]() {
        auto ret = RecyclableWrapper<StatesPlan>::createFromPoolRaw();
        return ret;
    }, discr);
    ++ret;
    return ret;
}

int goapLibInscribe(const std::string &discr) {
    return goapLibInscribeExplicit(Factory<IRoot>::singleton(), discr);
}

extern "C" {
int goapLibInscribe(const char *szDiscr) {
    return goapLibInscribeExplicit(Factory<IRoot>::singleton(), (szDiscr == nullptr) ? std::string() : szDiscr);
}
}
#endif // GOAPLIBINSCRIBE_CPP
