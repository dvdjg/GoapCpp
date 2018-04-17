#include <gmock/gmock.h>
#include "log_hook.h"
#include "time_utils.h"


using namespace goap;

class MyData
{
    //example data class, not used
};

class MyBuffer : public std::basic_streambuf<char, std::char_traits<char>>
{

public:

    inline MyBuffer(MyData data) :
        data(data)
    {
        setp(buf, buf + BUF_SIZE);
    }

protected:

    // For EOF detection
    typedef std::char_traits<char> Traits;

    // This is called when buffer becomes full. If
    // buffer is not used, then this is called every
    // time when characters are put to stream.
    inline virtual int overflow(int c = Traits::eof())
    {
#ifdef _DEBUG
        std::cerr << "(over)" << endl;
#endif
        // Handle output
        putChars(pbase(), pptr());
        if (c != Traits::eof())
        {
            char c2 = c;
            // Handle the one character that didn't fit to buffer
            putChars(&c2, &c2 + 1);
        }
        // This tells that buffer is empty again
        setp(buf, buf + BUF_SIZE);

        return c;
    }

    // This function is called when stream is flushed,
    // for example when std::endl is put to stream.
    inline virtual int sync(void)
    {
        // Handle output
        putChars(pbase(), pptr());
        // This tells that buffer is empty again
        setp(buf, buf + BUF_SIZE);
        return 0;
    }

private:

    // Work in buffer mode. It is also possible to work without buffer.
    static const size_t BUF_SIZE = 64;
    char buf[BUF_SIZE];

    // This is the example userdata
    MyData data;

    // In this function, the characters are parsed.
    inline void putChars(const char *begin, const char *end)
    {
#ifdef _DEBUG
        std::cerr << "(putChars(" << static_cast<const void *>(begin) <<
                  "," << static_cast<const void *>(end) << "))" << endl;
#endif
        //just print to stdout for now
        for (const char *c = begin; c < end; c++)
        {
            std::cout << *c;
        }
    }

};

class MyOStream : public std::basic_ostream< char, std::char_traits< char >>
{

public:

    inline MyOStream(MyData data = {}) :
        std::basic_ostream< char, std::char_traits< char >>(&buf),
                buf(data)
    {
    }
private:

    MyBuffer buf;
};




class LogTest : public ::testing::Test
{
public:
    LogTest()
    {
    }

    static void defaultLogHead(LOG &log) {
        char result[32];
        log << goap::nowTime(result) << " ";
        log << "[" << LOG::getLabel(log.getDebugLevel()) << "] ";
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

    LOG_CONF::singleton()._level = DEBUG;

    LOG(DEBUG) << "Second DEBUG";
    LOG(INFO) << "Second INFO";
    LOG(WARN) << "Second WARN";
    LOG(ERROR) << "Second ERROR";
}

static ostream& getMyOStream() {
    static MyOStream myOStream;
    return myOStream;
}

TEST_F(LogTest, MyOStream)
{
    ;
    LOG_CONF::singleton()._level = DEBUG;
    LOG_CONF::singleton().afnOstr[1] = getMyOStream;

    LOG(DEBUG) << "Third DEBUG";
    LOG(INFO) << "Third INFO";
    LOG(WARN) << "Third WARN";
    LOG(ERROR) << "Third ERROR";
}
