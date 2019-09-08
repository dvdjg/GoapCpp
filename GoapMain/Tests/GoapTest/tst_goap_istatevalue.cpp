#include <gmock/gmock.h>
#include "explicit_ptr.h"

#include "goap/istatevalue.h"
#include "goap/iscopetimer.h"

#include "newptr.h"


using namespace goap;

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
    ptrState->setAt(0, 0.0f);
    ptrState->setAt(1, 1.0);
    ptrState->setAt(2, 2.0);
    ptrState->setAt(3, 9.0);

    EXPECT_EQ(1.0, ptrState->at(1));
    EXPECT_EQ(0.0, ptrState->at(10));
    EXPECT_FLOAT_EQ(1.5, ptrState->at(1.5f)) << "La interpolación no coincide.";
}

TEST_F(GoapIStateValueTest, TestSetAtF)
{
    NewPtr<IStateValue> ptrState;
    EXPECT_TRUE(0 == ptrState->size());
    ptrState->resize(10);
    ptrState->setAt(1, 12.0); // Auto grow
    ptrState->setAt(2, 11.0);
    ptrState->setAt(2, 13.0);
    EXPECT_EQ(13.0, ptrState->at(2));
    EXPECT_FLOAT_EQ(12.5, ptrState->at(1.5f)) << "La interpolación no coincide.";
}


TEST_F(GoapIStateValueTest, TestInterpolateF)
{
    //auto init = {1.f, 2.f, 4.f, 8.f};
    //NewPtr<IStateValue> ptrState({}, init); // <std::string, std::initializer_list<float>>
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState;
    ptrStateOther->assign({3.f, 6.f, 12.f, 24.f}); //  --> {3.f, x, 6.f, x, 12.f, x, 24.f}
    ptrState->resize(7);
    ptrState->interpolateFrom(ptrStateOther);
    std::cout << ptrStateOther->toDebugString() << " converted to " << ptrState->toDebugString() << std::endl;
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
    std::cout << ptrStateOther->toDebugString() << " converted to " << ptrState->toDebugString() << std::endl;
    EXPECT_FLOAT_EQ(3.f, ptrState->at(0));
    EXPECT_FLOAT_EQ(24.f, ptrState->at(46));
    ptrState->setAt(45, 0.f);
    EXPECT_FLOAT_EQ(12.f, ptrState->at(45.5f));
    ptrState->setAt(45, 23.f);
    EXPECT_FLOAT_EQ(23.5f, ptrState->at(45.5f));
}

TEST_F(GoapIStateValueTest, TestCosineDistance)
{
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState({1.f, 0.f, 0.f});

    ptrStateOther->assign({0.f, 1.f, 0.f});
    float fDistance = ptrState->cosineDistance(ptrStateOther);
    EXPECT_FLOAT_EQ(-0.5f, fDistance);

    ptrState->assign({1.f, 1.f, 0.f});
    ptrStateOther->assign({0.f, 1.f, 1.f});
    float fDistance2 = ptrState->cosineDistance(ptrStateOther);
    EXPECT_FLOAT_EQ(-0.5f, fDistance2);
}

TEST_F(GoapIStateValueTest, TestEquals)
{
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState;

    ptrState->assign({1.f, 0.f, 0.f});
    ptrStateOther->assign({0.f, 1.f, 0.f});
    EXPECT_FALSE(ptrState->equal(ptrStateOther));
    ptrStateOther->assign(ptrState);
    EXPECT_TRUE(ptrState->equal(ptrStateOther));
}

TEST_F(GoapIStateValueTest, TestHide)
{
    NewPtr<IStateValue> ptrState({}, static_cast<const std::string &>("Lo qué"));
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

    EXPECT_TRUE(ptrStateSrc->equal(ptrStateDst1.getCPtr()));
    EXPECT_TRUE(ptrStateSrc->equal(ptrStateDst2.getCPtr()));

    std::string strMessage = ptrStateSrc->toString();
    EXPECT_TRUE(strMessage == szMessage);

    std::cout << "Contains: " << strMessage << std::endl;
}

