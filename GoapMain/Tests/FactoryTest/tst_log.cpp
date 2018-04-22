#include <gmock/gmock.h>


#include "log_hook.h"
#include "basicostream.h"
#include "time_utils.h"

using namespace goap;

class LogTest : public ::testing::Test
{
public:
    LogTest()
    {
    }

    static void defaultLogHead(LOG &log) {
        char result[32];
        log << goap::nowTime(result) << " ";
        log << "[" << getLogLevelLabel(log.getDebugLevel()) << "] ";
    }
protected:
    static void SetUpTestCase()
    {
        LOG_CONF::singleton().setFnLogHead(&defaultLogHead);
    }

    static void TearDownTestCase()
    {
    }

    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};


TEST_F(LogTest, Basic)
{
    LOG(DEBUG) << "First DEBUG";
    LOG(INFO) << "First INFO";
    LOG(WARN) << "First WARN";
    LOG(ERROR) << "First ERROR";

    LOG_CONF::singleton().setLevel(DEBUG);

    LOG(DEBUG) << "Second DEBUG";
    LOG(INFO) << "Second INFO";
    LOG(WARN) << "Second WARN";
    LOG(ERROR) << "Second ERROR";
}

static ostream& getBasicOStream() {
    static BasicOStream myOStream;
    return myOStream;
}

TEST_F(LogTest, BasicOStream)
{
    LOG_CONF::singleton().setLevel(DEBUG);
    LOG_CONF::singleton().afnOstr[0] = getBasicOStream;

    LOG(DEBUG) << "Third DEBUG" << endl;
    LOG(INFO) << "Third INFO";
    LOG(WARN) << "Third WARN";
    LOG(ERROR) << "Third ERROR";
}
