#include <gmock/gmock.h>


#include "log_hook.h"
#include "basicostream.h"
#include "time_utils.h"
#include "factory.h"

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
    LOG_CONF::singleton().setOstrFunciton(getBasicOStream);

    LOG(DEBUG) << "Third DEBUG";
    LOG(INFO) << "Third INFO";
    LOG(WARN) << "Third WARN";
    LOG(ERROR) << "Third ERROR";
}

static ostream& getAdvancedOStream() {
    static BasicOStream myOStream(Factory<IRoot>::singleton().create<IBasicSink, const std::string &, const std::ostream &>("Collection", std::string("Main"), std::cerr));
    return myOStream;
}

TEST_F(LogTest, AdvancedOStream)
{
    auto p = Factory<IRoot>::singleton().create<IBasicSink, const std::string &, const std::ostream &>("Collection", std::string("Main"), std::cerr);
    LOG_CONF::singleton().setLevel(DEBUG);
    LOG_CONF::singleton().setOstrFunciton(getAdvancedOStream);

    LOG(DEBUG) << "Fourth DEBUG";
    LOG(INFO) << "Fourth INFO";
    LOG(WARN) << "Fourth WARN";
    LOG(ERROR) << "Fourth ERROR";
}
