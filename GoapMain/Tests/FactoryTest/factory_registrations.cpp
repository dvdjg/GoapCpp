#include "statevalue.h"
#include "factory.h"

using namespace goap;

void registration(Factory<IRoot> &factory)
{
    factory.inscribe<FactoryType::Default, IStateValue, StateValue>();
}
