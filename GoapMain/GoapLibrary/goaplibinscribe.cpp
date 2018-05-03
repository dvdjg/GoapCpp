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
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const StateValue &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const IStateValue::CPtr &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const std::string &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const char *>(discr);
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
    }, discr+".cout");
    ++ret;
    factory.inscribe<FactoryType::Default, IScopeTime>([](const char *szMessage = nullptr, IScopeTime::pfn_time pfnTime = nullptr, bool bOutOfScope = true) {
        auto ret = RecyclableWrapper<ScopeTime>::createFromPoolRaw();
        ret->setMessage(szMessage);
        ret->setPfnTime(pfnTime);
        ret->setMessageOnDelete(bOutOfScope);
        return ret;
    }, discr+".cout");
    ++ret;

    return ret;
}

int goapLibInscribe(const std::string &discr) {
    return goapLibInscribeExplicit(Factory<IRoot>::singleton(), discr);
}

int goapLibInscribe(const char *szDiscr) {
    return goapLibInscribeExplicit(Factory<IRoot>::singleton(), (szDiscr == nullptr) ? std::string() : szDiscr);
}

#endif // GOAPLIBINSCRIBE_CPP
