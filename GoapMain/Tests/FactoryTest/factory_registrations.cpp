#include "statevalue.h"
#include "factory.h"

using namespace goap;

void registration()
{
    auto &factory = *Factory<IRoot>::singleton();
    factory.inscribe<FactoryType::Default, IStateValue, StateValue>();
}
