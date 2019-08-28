#ifndef GOAPLIBINSCRIBE_CPP
#define GOAPLIBINSCRIBE_CPP

#include <string>
#include "factory.h"
#include "reuseobjectpool.h"
#include "common/iroot.h"
#include "basicsinkcollection.h"
#include "basicostreamsink.h"
#include "statevalue.h"
#include "scopetimeostream.h"
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

    factory.inscribe<FactoryType::Default, IStateValue>([](){ return RecyclableWrapper<StateValue>::createFromPoolRaw(); }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const IStateValue::CPtr &cptr){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->assign(cptr);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const std::string &str){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->fromString(str);
        return ptr;
    }, discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue>([](const char *sz){
        auto ptr = RecyclableWrapper<StateValue>::createFromPoolRaw();
        ptr->fromString(sz);
        return ptr;
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
    }, discr+"Cout");
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
