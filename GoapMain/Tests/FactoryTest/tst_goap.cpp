#include <gmock/gmock.h>
#include "goap/explicit_ptr.h"

#include "goap/istatevalue.h"
#include "factory.h"

void registration();

using namespace goap;

//typedef explicit_ptr<Factory<IRoot>> PtrIFactory;

//bool b = has_intrusive_ptr<Factory<IRoot>>::value;

// bool const val = has_global_void__intrusive_ptr_add_ref<Factory<IRefCounter>>::value; // && has_global_void__intrusive_ptr_release<Factory<IRoot>>::value;

class GoapTest : public ::testing::Test
{
public:
    GoapTest()
    {
    }

protected:
    static void SetUpTestCase()
    {
        registration();
    }

    static void TearDownTestCase()
    {
    }

    //PtrIFactory _factory;

    virtual void SetUp() {
        //_factory = new Factory<IRoot>();
    }
    virtual void TearDown() {
        //_factory.reset();
    }
};

auto &factory = *Factory<IRoot>::singleton();

void testMe(IRefCounter* p) {

}

TEST_F(GoapTest, Intefaces)
{
    IStateValue* pSV = static_cast<IStateValue*>(0);
    IRefCounter* pRC = pSV;
    testMe(pSV);
    auto bConv1 = std::is_base_of<IRefCounter, IStateValue>::value;
    auto bConv2 = std::is_base_of<IStringValue, IStateValue>::value;
    auto bConv3 = std::is_base_of<IRefCounter, IStringValue>::value;
//    auto bbConv1 = goap::is_base_of<IRefCounter, IStateValue>::value;
//    auto bbConv2 = goap::is_base_of<IStringValue, IStateValue>::value;
//    auto bbConv3 = goap::is_base_of<IRefCounter, IStringValue>::value;
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

TEST_F(GoapTest, Test1)
{
    auto smartCounted1 = factory.create<IStateValue>();
    smartCounted1->resize(10);
    smartCounted1->setAt(0, 0.0);
    smartCounted1->setAt(1, 1.0);
    smartCounted1->setAt(2, 2.0);

    EXPECT_EQ(1.0, smartCounted1->at(1));
}



