#include "gtest/gtest.h"
#include "explicit_ptr.h"
#include "log_hook.h"

#include "goap/istatevalue.h"
#include "goap/iscopetimer.h"

#include "newptr.h"


using namespace goap;
using namespace std;

class GoapIStateValueTest : public ::testing::Test
{
public:
    GoapIStateValueTest() {
    }

protected:
    static void SetUpTestCase() {
    }
    static void TearDownTestCase() {
    }
    virtual void SetUp() {
        LOG_CONF::singleton().setLevel(DEBUG);
    }
    virtual void TearDown() {
    }
};

TEST_F(GoapIStateValueTest, TestSetAt)
{
    auto scopeTimer = NewPtr<IScopeTime>({}, "GoapIStateValueTest, Test1 A: ", true);
    ASSERT_TRUE(scopeTimer);
    NewPtr<IStateValue> ptrState;
    ASSERT_TRUE(ptrState);
    scopeTimer.reset();
    scopeTimer = NewPtr<IScopeTime>({}, "GoapIStateValueTest, Test1: B", true);
    ptrState->resize(10);
    ptrState->put(0, 0.0f);
    ptrState->put(1, 1.0);
    ptrState->put(2, 2.0);
    ptrState->put(3, 9.0);

    EXPECT_EQ(1.0, ptrState->at(1));
    EXPECT_EQ(0.0, ptrState->at(10));
    EXPECT_FLOAT_EQ(1.5, ptrState->at(1.5f)) << "La interpolación no coincide.";
}

TEST_F(GoapIStateValueTest, TestSetAtF)
{
    NewPtr<IStateValue> ptrState;
    EXPECT_TRUE(0 == ptrState->size());
    ptrState->resize(10);
    ptrState->put(1, 12.0); // Auto grow
    ptrState->put(2, 11.0);
    ptrState->put(2, 13.0);
    EXPECT_EQ(13.0, ptrState->at(2));
    EXPECT_FLOAT_EQ(12.5, ptrState->at(1.5f)) << "La interpolación no coincide.";
}


TEST_F(GoapIStateValueTest, TestInterpolateF)
{
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState;
    ptrStateOther->assign({3.f, 6.f, 12.f, 24.f}); //  --> {3.f, x, 6.f, x, 12.f, x, 24.f}
    ptrState->resize(7);
    ptrState->interpolateFrom(ptrStateOther);
    LOG(INFO) << ptrStateOther->toDebugString() << " converted to " << ptrState->toDebugString() << endl;
    EXPECT_FLOAT_EQ(3.f, ptrState->at(0));
    EXPECT_FLOAT_EQ(4.5f, ptrState->at(1));
    EXPECT_FLOAT_EQ(6.f, ptrState->at(2));
    EXPECT_FLOAT_EQ(9.f, ptrState->at(3));
    EXPECT_FLOAT_EQ(12.f, ptrState->at(4));
    EXPECT_FLOAT_EQ(18.f, ptrState->at(5));
    EXPECT_FLOAT_EQ(24.f, ptrState->at(6));
}

TEST_F(GoapIStateValueTest, TestInterpolateF_2)
{
    auto init = {3.f, 6.f, 12.f, 24.f};
    NewPtr<IStateValue> ptrStateOther({}, init);
    NewPtr<IStateValue> ptrState;
    ptrState->resize(47);
    ptrState->interpolateFrom(ptrStateOther);
    LOG(INFO) << ptrStateOther->toDebugString() << " converted to " << ptrState->toDebugString() << endl;
    EXPECT_FLOAT_EQ(3.f, ptrState->at(0));
    EXPECT_FLOAT_EQ(24.f, ptrState->at(46));
    ptrState->put(45, 0.f);
    EXPECT_FLOAT_EQ(12.f, ptrState->at(45.5f));
    ptrState->put(45, 23.f);
    EXPECT_FLOAT_EQ(23.5f, ptrState->at(45.5f));
}

TEST_F(GoapIStateValueTest, TestCosineDistance)
{
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState({1.f, 0.f, 0.f});

    ptrStateOther->assign({0.f, 1.f, 0.f});
    float fDistance = ptrState->cosineDistance(ptrStateOther);
    EXPECT_FLOAT_EQ(0, fDistance); // Offset distance = -0.5f

    ptrStateOther->assign({10.f, 0.f, 0.f});
    float fDistance2 = ptrState->cosineDistance(ptrStateOther);
    EXPECT_FLOAT_EQ(1, fDistance2);

    float fMod1 = 0, fMod2 = 0;
    ptrState->assign({1.f, 1.f, 0.f});
    ptrStateOther->assign({0.f, 1.f, 1.f});
    float fDistance3 = ptrState->cosineDistance(ptrStateOther, &fMod1, &fMod2);
    EXPECT_FLOAT_EQ(0.5f, fDistance3); // Offset distance = -0.5f
    EXPECT_EQ(fMod1, fMod2);


    float fDistance4 = NewPtr<IStateValue>({1.f, 1.f, 1.f})->cosineDistance({15.f, 15.f, 15.f}, &fMod1, &fMod2);
    EXPECT_FLOAT_EQ(1, fDistance4);
    EXPECT_FLOAT_EQ(sqrt(3.f), fMod1);
    EXPECT_FLOAT_EQ(sqrt(15.f*15.f*3.f), fMod2);
}

TEST_F(GoapIStateValueTest, TestEquals)
{
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState;

    ptrState->assign({1.f, 0.f, 0.f});
    ptrStateOther->assign({0.f, 1.f, 0.f});
    EXPECT_FALSE(ptrState->equals(ptrStateOther));
    ptrStateOther->assign(ptrState);
    EXPECT_TRUE(ptrState->equals(ptrStateOther));
}

TEST_F(GoapIStateValueTest, TestHide)
{
    NewPtr<IStateValue> ptrState({}, static_cast<const string &>("Lo qué"));
    NewPtr<IStateValue> ptrStateBis({}, "Lo qué");
    ASSERT_TRUE(ptrState);
    ASSERT_TRUE(ptrStateBis);

    ASSERT_EQ(*ptrState, *ptrStateBis);

    auto size = ptrState->size();
    EXPECT_EQ(7, size);
    auto f0 = ptrState->at(0);
    EXPECT_EQ('L', f0);
    auto f1 = ptrState->at(1);
    EXPECT_EQ('o', f1);

    auto f5 = ptrState->at(5);
    EXPECT_EQ("é"[0], f5);

    auto f6 = ptrState->at(6);
    EXPECT_EQ("é"[1], f6);
}

TEST_F(GoapIStateValueTest, TestHide2)
{
    NewPtr<IStateValue> ptrState({}, "Lo qué");
    ASSERT_TRUE(ptrState);

    auto size = ptrState->size();
    EXPECT_EQ(7, size);
    auto f0 = ptrState->at(0);
    EXPECT_EQ('L', f0);
    auto f1 = ptrState->at(1);
    EXPECT_EQ('o', f1);

    auto f5 = ptrState->at(5);
    EXPECT_EQ("é"[0], f5);

    auto f6 = ptrState->at(6);
    EXPECT_EQ("é"[1], f6);
}

TEST_F(GoapIStateValueTest, Assign)
{
    const char *szMessage = "Mama mía";
    NewPtr<IStateValue> ptrStateSrc({}, szMessage);
    ASSERT_TRUE(ptrStateSrc);
    NewPtr<IStateValue> ptrStateDst1(ptrStateSrc.getPtr());
    ASSERT_TRUE(ptrStateDst1);
    NewPtr<IStateValue> ptrStateDst2(ptrStateSrc);
    ASSERT_TRUE(ptrStateDst2);

    EXPECT_TRUE(ptrStateSrc->equals(ptrStateDst1.getCPtr()));
    EXPECT_TRUE(ptrStateSrc->equals(ptrStateDst2.getCPtr()));

    string strMessage = ptrStateSrc->toString();
    EXPECT_TRUE(strMessage == szMessage);
    EXPECT_EQ(strMessage, szMessage);

    LOG(INFO) << "Contains: " << strMessage << endl;
}

TEST_F(GoapIStateValueTest, TestClone)
{
    NewPtr<IStateValue> ptrState({1.f, 0.5f, 0.f, 9.f, 98.f});

    auto ptrCloned = dynamic_pointer_cast<IStateValue>(ptrState->clone());
    ASSERT_TRUE(ptrCloned);
    ASSERT_EQ(*ptrCloned, *ptrState);
    ASSERT_EQ(9.f, ptrCloned->at(3));
}
