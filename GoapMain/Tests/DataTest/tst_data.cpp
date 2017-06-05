#include <gmock/gmock.h>


using namespace goap;



class DataTest : public ::testing::Test
{

public:
    DataTest()
    {
    }
protected:
    // Per-test-case set-up.
    // Called before the first test in this test case.
    // Can be omitted if not needed.
    static void SetUpTestCase()
    {
        //shared_resource_ = new ...;
    }

    // Per-test-case tear-down.
    // Called after the last test in this test case.
    // Can be omitted if not needed.
    static void TearDownTestCase()
    {
        //delete shared_resource_;
        //shared_resource_ = NULL;
    }

    // You can define per-test set-up and tear-down logic as usual.
    virtual void SetUp() {  }
    virtual void TearDown() {  }
};

TEST_F(DataTest, Test1)
{

    EXPECT_TRUE(true);
}

TEST_F(DataTest, Test2)
{

}
