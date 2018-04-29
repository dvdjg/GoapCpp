#include <string>
#include "statevalue.h"
#include "factory.h"

using namespace goap;

int goapLibInscribeExplicit(Factory<IRoot> & factory, const std::string &discr);


void registration(Factory<IRoot> &factory)
{
    factory.inscribe<FactoryType::Default, IStateValue, StateValue>();

    goapLibInscribeExplicit(factory, {});
}
