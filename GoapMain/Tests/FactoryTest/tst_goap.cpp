#include <gmock/gmock.h>
#include "explicit_ptr.h"

#include "goap/istatevalue.h"
#include "goap/iscopetimer.h"

#include "factory.h"


using namespace goap;
void registration(Factory<IRoot> &factory);

class FactoryGoapTest : public ::testing::Test
{
public:
    FactoryGoapTest()
    {
    }

protected:
    static void SetUpTestCase() {
    }

    static void TearDownTestCase() {
    }

    explicit_ptr<Factory<IRoot>> _ptrFactory;
    Factory<IRoot>& factory() {
        return *_ptrFactory;
    }

    virtual void SetUp() {
        _ptrFactory.reset(new Factory<IRoot>());
        registration(factory());
    }
    virtual void TearDown() {
        _ptrFactory.reset();
    }
};

TEST_F(FactoryGoapTest, Intefaces)
{
    auto bConv1 = std::is_base_of<IRefCounter, IStateValue>::value;
    auto bConv2 = std::is_base_of<IStringValue, IStateValue>::value;
    auto bConv3 = std::is_base_of<IRefCounter, IStringValue>::value;

    auto bIRoot = has_intrusive_ptr<IRoot>::value;
    auto bIRefCounter = has_intrusive_ptr<IRefCounter>::value;
    auto bIStateValue = has_intrusive_ptr<IStateValue>::value;

    EXPECT_FALSE(bIRoot);
    EXPECT_TRUE(bConv1);
    EXPECT_TRUE(bConv2);
    EXPECT_TRUE(bConv3);
    EXPECT_TRUE(bIRefCounter);
    EXPECT_TRUE(bIStateValue);
}

TEST_F(FactoryGoapTest, Test1)
{
    auto scopeTimer = factory().create<IScopeTime>({}, static_cast<const char *>("FactoryGoapTest.Test1"), true);
    ASSERT_TRUE(scopeTimer);
    auto smartCounted1 = factory().create<IStateValue>();
    ASSERT_TRUE(smartCounted1);

    smartCounted1->resize(10);
    smartCounted1->put(0, 0.0);
    smartCounted1->put(1, 1.0);
    smartCounted1->put(2, 2.0);

    EXPECT_EQ(1.0, smartCounted1->at(1));
}



