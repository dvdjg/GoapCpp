#ifndef GOAPLIBINSCRIBE_CPP
#define GOAPLIBINSCRIBE_CPP

#include <string>
#include "goaplibinscribe.h"
#include "factory.h"
#include "common/iroot.h"
#include "basicsinkcollection.h"
#include "basicostreamsink.h"
#include "statevalue.h"
#include "scopetime.h"

using namespace goap;

int goapLibInscribeExplicit(Factory<IRoot> & factory = Factory<IRoot>::singleton(), const std::string &discr = {}) {
    int ret = 0;
    factory.inscribe<FactoryType::Default, IBasicSinkCollection, BasicSinkCollection>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSinkCollection, BasicSinkCollection, const std::string &, IBasicSink::Ptr>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSinkCollection, BasicSinkCollection, const std::string &, const std::ostream &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicSinkCollection>(discr+"Collection");
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicSinkCollection, const std::string &, IBasicSink::Ptr>(discr+"Collection");
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicSinkCollection, const std::string &, const std::ostream &>(discr+"Collection");
    ++ret;
    factory.inscribe<FactoryType::Default, IBasicSink, BasicOstreamSink>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const StateValue &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IStateValue, StateValue, const std::string &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IScopeTime, ScopeTime, const std::string &, std::ostream &>(discr);
    ++ret;
    factory.inscribe<FactoryType::Default, IScopeTime, ScopeTime, const char *, std::ostream &>(discr);
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
