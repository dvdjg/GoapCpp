#include <gmock/gmock.h>
#include "log_hook.h"


using namespace goap;

class LogTest : public ::testing::Test
{
public:
    LogTest()
    {
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

TEST_F(LogTest, Basic)
{
    LOG(ERROR) << "First ERROR";
    LOG(WARN) << "First WARN";

}



