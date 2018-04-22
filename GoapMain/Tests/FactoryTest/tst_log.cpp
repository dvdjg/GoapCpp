#include <gmock/gmock.h>
#include <mutex>
#include "goap/ibasicsink.h"
#include "log_hook.h"
#include "time_utils.h"


using namespace goap;

class BasicOstreamSink : public IBasicSink
{
    std::basic_ostream< char, std::char_traits< char >> &_ostream;
public:
    typedef std::shared_ptr<BasicOstreamSink> Ptr;

    BasicOstreamSink(const BasicOstreamSink& o);
    BasicOstreamSink(std::basic_ostream< char, std::char_traits< char >> &ostream = std::cerr);
    virtual ~BasicOstreamSink();

    // IBasicSink interface
    void write(const char *begin, const char *end) override;
    void flush() override;
    void eof() const override;
};

BasicOstreamSink::BasicOstreamSink(const BasicOstreamSink &o) : _ostream(o._ostream)
{
}

BasicOstreamSink::BasicOstreamSink(std::basic_ostream<char, std::char_traits<char> > &ostream) : _ostream(ostream)
{
}

BasicOstreamSink::~BasicOstreamSink()
{
    flush();
}

void BasicOstreamSink::write(const char *begin, const char *end)
{
    _ostream.write(begin, end - begin);
}

void BasicOstreamSink::flush()
{
    _ostream.flush();
}

void BasicOstreamSink::eof() const
{
    _ostream.eof();
}

class BasicStreamBuffer : public std::basic_streambuf<char, std::char_traits<char>>
{
    std::mutex _mutex;
public:

    BasicStreamBuffer(IBasicSink::Ptr data) :
        _data(data)
    {
        setp(_buf, _buf + BUF_SIZE);
    }

    ~BasicStreamBuffer()
    {
        sync();
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
        std::lock_guard<std::mutex> lock(_mutex);
        write(pbase(), pptr());
        if (c != Traits::eof()) {
            char c2 = c;
            // Handle the one character that didn't fit to buffer
            write(&c2, &c2 + 1);
        } else {
            _data->eof();
        }
        // This tells that buffer is empty again
        setp(_buf, _buf + BUF_SIZE);

        return c;
    }

    // This function is called when stream is flushed,
    // for example when std::endl is put to stream.
    inline virtual int sync(void)
    {
        // Handle output
        std::lock_guard<std::mutex> lock(_mutex);
        write(pbase(), pptr());
        _data->flush();
        // This tells that buffer is empty again
        setp(_buf, _buf + BUF_SIZE);
        return 0;
    }

private:

    // Work in buffer mode. It is also possible to work without buffer.
    static const size_t BUF_SIZE = 128;
    char _buf[BUF_SIZE];

    IBasicSink::Ptr _data;

    // In this function, the characters are parsed.
    inline void write(const char *begin, const char *end)
    {
#ifdef _DEBUG
        std::cerr << "(putChars(" << static_cast<const void *>(begin) <<
                  "," << static_cast<const void *>(end) << "))" << endl;
#endif
        _data->write(begin, end);
    }

};

class BasicOStream : public std::basic_ostream< char, std::char_traits< char >>
{

public:

    BasicOStream(IBasicSink::Ptr data) :
        std::basic_ostream< char, std::char_traits< char >>(&_buf),
                _buf(data)
    {
    }
    BasicOStream() :
        std::basic_ostream< char, std::char_traits< char >>(&_buf),
                _buf(IBasicSink::Ptr(new BasicOstreamSink()))
    {
    }
private:

    BasicStreamBuffer _buf;
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
