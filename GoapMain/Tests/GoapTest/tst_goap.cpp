#include <gmock/gmock.h>
#include "explicit_ptr.h"

#include "goap/istatevalue.h"
#include "goap/iscopetimer.h"

#include "factory.h"
#include "newptr.h"


using namespace goap;

class GoapTest : public ::testing::Test
{
public:
    GoapTest() {
    }

    Factory<IRoot>& factory() {
        return Factory<IRoot>::singleton();
}

protected:
    static void SetUpTestCase() {
    }
    static void TearDownTestCase() {
    }
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};


TEST_F(GoapTest, Test1)
{
    auto scopeTimer = factory().create<IScopeTime, const std::string &, std::ostream &>({}, "Test1", std::cerr);
    ASSERT_TRUE(scopeTimer);
    auto ptrState = factory().create<IStateValue>();
    ASSERT_TRUE(ptrState);

    ptrState->resize(10);
    ptrState->setAt(0, 0.0f);
    ptrState->setAt(1, 1.0);
    ptrState->setAt(2, 2.0);

    EXPECT_EQ(1.0, ptrState->at(1));
}

TEST_F(GoapTest, TestHide)
{
    NewPtr<IStateValue> ptrState(static_cast<const std::string &>("Lo qué"));
    ASSERT_TRUE(ptrState);

    ptrState->resize(10);
    ptrState->setAt(0, 0.0);
    ptrState->setAt(1, 1.0);
    ptrState->setAt(2, 2.0);

    EXPECT_EQ(1.0, ptrState->at(1));
}

