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

TEST_F(GoapTest, TestSetAt)
{
    auto scopeTimer = NewPtr<IScopeTime>({}, (const char *) "GoapTest, Test1 A: ", true);
    ASSERT_TRUE(scopeTimer);
    NewPtr<IStateValue> ptrState;
    ASSERT_TRUE(ptrState);
    scopeTimer.reset();
    scopeTimer = NewPtr<IScopeTime>({}, (const char *) "GoapTest, Test1: B", true);
    ptrState->resize(10);
    ptrState->setAt(0, 0.0f);
    ptrState->setAt(1, 1.0);
    ptrState->setAt(2, 2.0);
    ptrState->setAt(3, 9.0);

    EXPECT_EQ(1.0, ptrState->at(1));
    EXPECT_EQ(0.0, ptrState->at(10));
    EXPECT_FLOAT_EQ(1.5, ptrState->atF(1.5)) << "La interpolación no coincide.";
}

TEST_F(GoapTest, TestSetAtF)
{
    NewPtr<IStateValue> ptrState;
    EXPECT_EQ(0, ptrState->size());
    ptrState->resize(10);
    ptrState->setAt(1, 12.0); // Auto grow
    ptrState->setAt(2, 11.0);
    ptrState->setAtF(2, 13.0);
    EXPECT_EQ(13.0, ptrState->at(2));
    EXPECT_FLOAT_EQ(12.5, ptrState->atF(1.5)) << "La interpolación no coincide.";
}


TEST_F(GoapTest, TestinterpolateF)
{
    //auto init = {1.f, 2.f, 4.f, 8.f};
    //NewPtr<IStateValue> ptrState({}, init); // <std::string, std::initializer_list<float>>
    NewPtr<IStateValue> ptrStateOther;
    NewPtr<IStateValue> ptrState;
    ptrStateOther->assign({3.f, 6.f, 12.f, 24.f}); //  --> {3.f, x, 6.f, x, 12.f, x, 24.f}
    ptrState->resize(7);
    ptrState->interpolateFrom(ptrStateOther);
    std::cout << ptrStateOther->toString() << " converted to " << ptrState->toString();
    EXPECT_EQ(3.f, ptrState->at(0));
    EXPECT_EQ(4.5f, ptrState->at(1));
    EXPECT_EQ(6.f, ptrState->at(2));
    EXPECT_EQ(9.f, ptrState->at(3));
    EXPECT_EQ(12.f, ptrState->at(4));
    EXPECT_EQ(18.f, ptrState->at(5));
    EXPECT_EQ(24.f, ptrState->at(6));
}

TEST_F(GoapTest, TestinterpolateF_2)
{
    auto init = {3.f, 6.f, 12.f, 24.f};
    NewPtr<IStateValue> ptrStateOther({}, init);
    NewPtr<IStateValue> ptrState;
    ptrState->resize(4*12-1);
    ptrState->interpolateFrom(ptrStateOther);
    std::cout << ptrStateOther->toString() << " converted to " << ptrState->toString();
    EXPECT_EQ(3.f, ptrState->at(0));
    //EXPECT_EQ(4.5f, ptrState->at(1));
    //EXPECT_EQ(6.f, ptrState->at(2));
    //EXPECT_EQ(9.f, ptrState->at(3));
    //EXPECT_EQ(12.f, ptrState->at(4));
    //EXPECT_EQ(18.f, ptrState->at(5));
    EXPECT_EQ(24.f, ptrState->at(46));
}

TEST_F(GoapTest, TestHide)
{
    NewPtr<IStateValue> ptrState({}, static_cast<const std::string &>("Lo qué"));
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

TEST_F(GoapTest, TestHide2)
{
    NewPtr<IStateValue> ptrState({}, static_cast<const char *>("Lo qué"));
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

TEST_F(GoapTest, Assign)
{
    NewPtr<IStateValue> ptrStateSrc({}, static_cast<const char *>("Mama mía"));
    ASSERT_TRUE(ptrStateSrc);
    NewPtr<IStateValue> ptrStateDst1(ptrStateSrc.getPtr());
    ASSERT_TRUE(ptrStateDst1);
    NewPtr<IStateValue> ptrStateDst2(ptrStateSrc);
    ASSERT_TRUE(ptrStateDst2);

    EXPECT_TRUE(ptrStateSrc->equal(ptrStateDst1.getCPtr()));
    EXPECT_TRUE(ptrStateSrc->equal(ptrStateDst2.getCPtr()));
}

