#include <gmock/gmock.h>
#include "explicit_ptr.h"

#include "istatevalue.h"
#include "factory.h"

void registration();

using namespace goap;

//typedef explicit_ptr<Factory<IRoot>> PtrIFactory;

//bool b = has_intrusive_ptr<Factory<IRoot>>::value;

 bool const val = has_global_void__intrusive_ptr_add_ref<Factory<IRefCounter>>::value; // && has_global_void__intrusive_ptr_release<Factory<IRoot>>::value;

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


TEST_F(GoapTest, Test1)
{
    auto smartCounted1 = factory.create<IStateValue>();
    smartCounted1->resize(10);
    smartCounted1->setAt(0, 0.0);
    smartCounted1->setAt(1, 1.0);
    smartCounted1->setAt(2, 2.0);

    EXPECT_EQ(1.0, smartCounted1->at(1));
}



