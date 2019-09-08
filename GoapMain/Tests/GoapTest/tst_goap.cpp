#include <gmock/gmock.h>
#include "explicit_ptr.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "newptr.h"


using namespace goap;

class GoapIStateTest : public ::testing::Test
{
public:
    GoapIStateTest() {
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

TEST_F(GoapIStateTest, TestSetAt)
{
    auto scopeTimer = NewPtr<IScopeTime>({}, (const char *) "GoapIStateTest, Test1 A: ", true);
    ASSERT_TRUE(scopeTimer);
    NewPtr<IStateValue> ptrState;
    ASSERT_TRUE(ptrState);
    scopeTimer.reset();
    scopeTimer = NewPtr<IScopeTime>({}, (const char *) "GoapIStateTest, Test1: B", true);
    ptrState->resize(10);
    ptrState->setAt(0, 0.0f);
    ptrState->setAt(1, 1.0);
    ptrState->setAt(2, 2.0);
    ptrState->setAt(3, 9.0);

    EXPECT_EQ(1.0, ptrState->at(1));
    EXPECT_EQ(0.0, ptrState->at(10));
    EXPECT_FLOAT_EQ(1.5, ptrState->at(1.5f)) << "La interpolación no coincide.";
}
