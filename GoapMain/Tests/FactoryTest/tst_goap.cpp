#include <gmock/gmock.h>
#include <boost/intrusive_ptr.hpp>

#include "istatevalue.h"
#include "factory.h"

void registration();

using namespace goap;

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

    virtual void SetUp() {  }
    virtual void TearDown() {  }
};


TEST_F(GoapTest, Test1)
{
    auto &factory = *Factory<IRoot>::singleton();
    auto smartCounted1 = factory.create<IStateValue>({});
    smartCounted1->resize(10);
    smartCounted1->setAt(0, 0.0);
    smartCounted1->setAt(1, 1.0);
    smartCounted1->setAt(2, 2.0);

    EXPECT_EQ(1.0, smartCounted1->at(1));
}



