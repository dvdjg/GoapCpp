#include <gtest/gtest.h>

#include <functional>
#include <future>
#include <mutex>
#include "common/irefcounter.h"
#include "refcounter.h"
#include "factory.h"
#include "reuseobjectpool.h"
#include "scopetimeostream.h"

using namespace goap;

namespace goap_boost
{

class IStringData : public IRefCounter
{
public:
    virtual const std::string &getData() const = 0;
    virtual void setData(const std::string &getData) = 0;
};

class IStringDataFromRoot : public IRoot
{
public:
    virtual const std::string &getData() const = 0;
    virtual void setData(const std::string &getData) = 0;
};

/*
    class MockIRefCounter : public IRefCounter
    {
    public:
    MOCK_CONST_METHOD0(loadRef, int());
    MOCK_CONST_METHOD0(addRef, int());
    MOCK_CONST_METHOD0(releaseRef, int());
    MOCK_METHOD0(suicide, void());
    };

    class MockIStringData : public MockIRefCounter
    {
    public:
    MOCK_METHOD1(setData, void(const std::string &getData));
    MOCK_CONST_METHOD0(getData, const std::string & ());
    };
*/

class NonCounted : public IStringDataFromRoot
{
    std::string _data;
public:
    NonCounted(const std::string &data = {}) : _data(data)
    {
        std::cerr << "[          ] " << "Created " << _data << std::endl;
    }
    NonCounted(int number) : _data(std::to_string(number))
    {
        std::cerr << "[          ] " << "Created " << _data << std::endl;
    }
    ~NonCounted()
    {
        std::cerr << "[          ] " << "Destroyed " << _data << std::endl;
    }
    const std::string &getData() const override
    {
        return _data;
    }
    void setData(const std::string &data) override
    {
        _data = data;
    }
    void clear()
    {
        _data = "<<" + _data + ">>";
    }
};

NonCounted *createCountedFromRoot(const std::string &str)
{
    return new NonCounted(str);
}


class Counted : public IStringData
{
    IMPLEMENT_REFCOUNTER()

    std::string _data;
public:
    Counted(const std::string &data = {}) : _data(data)
    {
        std::cerr << "[          ] " << "Created " << _data << std::endl;
    }
    Counted(int number) : _data(std::to_string(number))
    {
        std::cerr << "[          ] " << "Created " << _data << std::endl;
    }
    ~Counted()
    {
        std::cerr << "[          ] " << "Destroyed " << _data << std::endl;
    }
    const std::string &getData() const override
    {
        return _data;
    }
    void setData(const std::string &data) override
    {
        _data = data;
    }
};

Counted *createCounted(const std::string &str)
{
    return new Counted(str);
}


class FactoryAllTest : public ::testing::Test
{

public:
    FactoryAllTest()
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

}

using namespace goap_boost;

TEST_F(FactoryAllTest, Test1)
{
    Factory<IRoot> factory;

    auto lstr = [](const std::string & str)
    {
        return new Counted(str);
    };
    bool bInscribed = factory.isInscribed<IStringData, const std::string &>();
    EXPECT_FALSE(bInscribed);

    bInscribed = factory.isInscribed<IStringData>("Singleton");
    EXPECT_FALSE(bInscribed);

    std::function<Counted* (const std::string &str)> f = lstr;
    factory.inscribe<FactoryType::Default, IStringData>(&createCounted);
    factory.inscribe<FactoryType::Default, IStringData>(f, "Functor");
    factory.inscribe<FactoryType::Default, IStringData>(lstr, "Lambda");
    factory.inscribe<FactoryType::Default, IStringData, Counted, const std::string &>("Delegate");
    factory.inscribe<FactoryType::Singleton, IStringData>([]()
    {
        return new Counted("The singleton");
    }, "Singleton");
    bInscribed = factory.isInscribed<IStringData, const std::string &>();
    EXPECT_TRUE(bInscribed);
    bInscribed = factory.isInscribed<IStringData>("Singleton");
    EXPECT_TRUE(bInscribed);
    Counted counted("Hola");
    {
        ScopeTimeOstream<std::cerr> scope(R"(Counted counted("Hola");)");
        explicit_ptr<Counted> ptrCounted(new Counted("Hello"));
        {
            auto smartCounted1 = factory.create<IStringData, const std::string &>({}, "Hallo");
            auto smartCounted2 = factory.create<IStringData, const std::string &>("Lambda", "Haloha");
            auto smartCounted3 = factory.create<IStringData, const std::string &>("Functor", "Jalo");
            auto smartCounted4 = factory.create<IStringData, const std::string &>("Delegate", "Hi");
            auto smartCounted5 = factory.create<IStringData>("Singleton");
            auto smartCounted6 = factory.create<IStringData>("Singleton");
            if (smartCounted1)
            {
                std::cerr << "[          ] " << smartCounted1->getData() << std::endl;
            }
            //smartCounted->suicide();
        }
    }
    factory.inscribe<FactoryType::Singleton, IStringDataFromRoot>([]()
    {
        return new NonCounted("The singleton from root");
    }, "Singleton");
    factory.inscribe<FactoryType::Default, IStringDataFromRoot>([](const std::string & str)
    {
        return new NonCounted(str);
    });
    factory.inscribe<FactoryType::Default, IStringDataFromRoot>([](int number)
    {
        return new NonCounted(number);
    });
    {
        ScopeTimeOstream<std::cerr> scope(R"(smartCounted)");
        auto smartCounted = factory.create<IStringDataFromRoot, const std::string &>({}, "Bye");
        auto smartCounted2 = factory.create<IStringDataFromRoot>({}, 123);
        auto smartCounted3 = factory.create<IStringDataFromRoot>("Singleton");
        auto smartCounted4 = factory.create<IStringDataFromRoot>("Singleton");
        if (smartCounted)
        {
            std::cerr << "[          ] " << smartCounted->getData() << std::endl;
        }
    }

    typedef RecyclableWrapper<Counted> RecyclableCounted;
    {
        ScopeTimeOstream<std::cerr> scope(R"(RecyclableCounted::createFromPool())");
        auto fromPool1(RecyclableCounted::createFromPool());
        auto fromPool2(RecyclableCounted::createFromPool());
        auto fromPool3(RecyclableCounted::createFromPool());
        fromPool1->setData("Pooled1");
        fromPool2->setData("Pooled2");
        fromPool3->setData("Pooled3");
    }
    {
        auto fromPool1(RecyclableCounted::createFromPool());
        auto fromPool2(RecyclableCounted::createFromPool());
        auto fromPool3(RecyclableCounted::createFromPool());
        std::cerr << "[          ] " << fromPool1->getData() << std::endl;
        std::cerr << "[          ] " << fromPool2->getData() << std::endl;
        std::cerr << "[          ] " << fromPool3->getData() << std::endl;
    }

    typedef RecyclableWrapper<NonCounted> RecyclableNonCounted;
    {
        auto fromPool1(RecyclableNonCounted::createFromPool());
        auto fromPool2(RecyclableNonCounted::createFromPool());
        auto fromPool3(RecyclableNonCounted::createFromPool());
        fromPool1->setData("PooledFromRoot1");
        fromPool2->setData("PooledFromRoot2");
        fromPool3->setData("PooledFromRoot3");
    }
    {
        auto fromPool1(RecyclableNonCounted::createFromPool());
        auto fromPool2(RecyclableNonCounted::createFromPool());
        auto fromPool3(RecyclableNonCounted::createFromPool());
        std::cerr << "[          ] " << fromPool1->getData() << std::endl;
        std::cerr << "[          ] " << fromPool2->getData() << std::endl;
        std::cerr << "[          ] " << fromPool3->getData() << std::endl;
    }
    factory.inscribe<FactoryType::Default, IStringData>([]()
    {
        return RecyclableCounted::createFromPoolRaw();
    }, "Pool");
    {
        auto fromPool1 = factory.create<IStringData>("Pool");
        auto fromPool2 = factory.create<IStringData>("Pool");
        auto fromPool3 = factory.create<IStringData>("Pool");
        fromPool1->setData("FactoryPooled1");
        fromPool2->setData("FactoryPooled2");
        fromPool3->setData("FactoryPooled3");
    }
    {
        auto fromPool1 = factory.create<IStringData>("Pool");
        auto fromPool2 = factory.create<IStringData>("Pool");
        auto fromPool3 = factory.create<IStringData>("Pool");
        fromPool1->setData(fromPool1->getData() + " - FactoryPooled1");
        fromPool2->setData(fromPool2->getData() + " - FactoryPooled2");
        fromPool3->setData(fromPool3->getData() + " - FactoryPooled3");
    }
    factory.inscribe<FactoryType::Default, IStringDataFromRoot>([]()
    {
        return RecyclableNonCounted::createFromPoolRaw();
    }, "Pool");
    {
        auto fromPool1 = factory.create<IStringDataFromRoot>("Pool");
        auto fromPool2 = factory.create<IStringDataFromRoot>("Pool");
        auto fromPool3 = factory.create<IStringDataFromRoot>("Pool");
        fromPool1->setData("FactoryPooledFromRoot1");
        fromPool2->setData("FactoryPooledFromRoot2");
        fromPool3->setData("FactoryPooledFromRoot3");
    }
    auto created = factory.createAll<IStringDataFromRoot, const std::string &>("Datos");
    std::cerr << "[          ] " << "Cleanup" << std::endl;
    RecyclableCounted::getPool()->clear();
    RecyclableNonCounted::getPool()->clear();
    std::cerr << "[          ] " << "RecyclableCounted: " << RecyclableCounted::getPool()->avoidedAllocations() << std::endl;
    std::cerr << "[          ] " << "RecyclableNonCounted: " << RecyclableNonCounted::getPool()->avoidedAllocations() << std::endl;
    EXPECT_TRUE(true);
}

TEST_F(FactoryAllTest, Futures)
{
    std::mutex mtx;
    std::cout << "Main thread id: " << std::this_thread::get_id()
              << std::endl;
    std::vector<std::future<std::thread::id>> futures;
    for (int i = 0; i < 20; ++i)
    {
        auto fut = std::async(std::launch::async, [&](int counter)
        {
            std::thread::id id = std::this_thread::get_id();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Inside thread [" << counter << "] " << id << "; \n";
            return id;
        }, i);
        futures.push_back(std::move(fut));
    }
    for (auto &fut : futures)
    {
        // fut.wait();
        while (fut.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready)
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "... still not ready" << std::endl;
        }
        std::thread::id id = fut.get();
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Finished " << id << std::endl;
    }
    std::cout << std::endl;
}

/*
    class Turtle
    {
    public:
    virtual ~Turtle() {}
    virtual void PenUp() = 0;
    virtual void PenDown() = 0;
    virtual void Forward(int distance) = 0;
    virtual void Turn(int degrees) = 0;
    virtual void GoTo(int x, int y) = 0;
    virtual int GetX() const = 0;
    virtual int GetY() const = 0;
    };

    class MockTurtle : public Turtle
    {
    public:
    MOCK_METHOD0(PenUp, void());
    MOCK_METHOD0(PenDown, void());
    MOCK_METHOD1(Forward, void(int distance));
    MOCK_METHOD1(Turn, void(int degrees));
    MOCK_METHOD2(GoTo, void(int x, int y));
    MOCK_CONST_METHOD0(GetX, int());
    MOCK_CONST_METHOD0(GetY, int());
    };

    class Painter
    {
    Turtle* turtle;
    public:
    Painter(Turtle* turtle) : turtle(turtle)
    {
    }

    bool DrawCircle(int x, int y, int)
    {
        turtle->GoTo(x, y);
        turtle->PenDown();

        return true;
    }
    };

    TEST(PainterTest, CanDrawSomething)
    {
    MockTurtle turtle;

    // Set expectations
    EXPECT_CALL(turtle, GoTo(100, 50));
    EXPECT_CALL(turtle, PenDown());

    // Call sequence
    Painter painter(&turtle);

    EXPECT_TRUE(painter.DrawCircle(100, 50, 10));
    }

*/
