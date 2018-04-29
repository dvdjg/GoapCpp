#include <gtest/gtest.h>
#include "factory.h"
#include "common/iroot.h"
#include "goaplibinscribe.h"

using namespace goap;

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    goapLibInscribe();

    int ret = RUN_ALL_TESTS();
    Factory<IRoot>::singleton().clear();
    return ret;
}
