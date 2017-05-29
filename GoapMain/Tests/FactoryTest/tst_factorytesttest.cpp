#include <gmock/gmock.h>

#include <functional>
#include "factory.h"
#include "irefcounter.h"
#include "refcounter.h"
#include "reuseobjectpool.h"


int main(int argc, char **argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

using namespace goap;

class IStringData : virtual public IRefCounter
{
public:
    virtual const std::string &getData() const = 0;
    virtual void setData(const std::string &getData) = 0;
};

class IStringDataFromRoot : virtual public IRoot
{
public:
    virtual const std::string &getData() const = 0;
    virtual void setData(const std::string &getData) = 0;
};

class CountedFromRoot : virtual public IStringDataFromRoot
{
    std::string _data;
public:
    CountedFromRoot(const std::string &data = {}) : _data(data)
    {
        std::cerr << "[          ] " << "Created " << _data << std::endl;
    }
    CountedFromRoot(int number) : _data(std::to_string(number))
    {
        std::cerr << "[          ] " << "Created " << _data << std::endl;
    }
    ~CountedFromRoot()
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

CountedFromRoot *createCountedFromRoot(const std::string &str)
{
    return new CountedFromRoot(str);
}


class Counted : virtual public IStringData
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


class FactoryTestTest : public ::testing::Test
{

public:
    FactoryTestTest()
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
TEST_F(FactoryTestTest, Test1)
{
    auto &factory = Factory<IRoot>::singleton();

    //auto l = []() {return new Counted;};
    auto lstr = [](const std::string & str)
    {
        return new Counted(str);
    };
    std::function<Counted* (const std::string &str)> f = lstr;
    //Factory<IRoot>::singleton().Register<IStringData, Counted>([](int i) { return new Counted("Dentro");});
    factory.inscribe<FactoryType::Default, IStringData>(&createCounted);
    factory.inscribe<FactoryType::Default, IStringData>(f, "Functor");
    factory.inscribe<FactoryType::Default, IStringData>(lstr, "Lambda");
    factory.inscribe<FactoryType::Default, IStringData, Counted, const std::string &>("Delegate");
    factory.inscribe<FactoryType::Singleton, IStringData>([]()
    {
        return new Counted("The singleton");
    }, "Singleton");
    Counted counted("Hola");
    {
        SmartPointerChooser<Counted>::type ptrCounted(new Counted("Hello"));
        {
            auto smartCounted = factory.create<IStringData, const std::string &>({}, "Hallo");
            auto smartCounted2 = factory.create<IStringData, const std::string &>("Lambda", "Haloha");
            auto smartCounted3 = factory.create<IStringData, const std::string &>("Functor", "Jalo");
            auto smartCounted4 = factory.create<IStringData, const std::string &>("Delegate", "Hi");
            auto smartCounted5 = factory.create<IStringData>("Singleton");
            auto smartCounted6 = factory.create<IStringData>("Singleton");
            if (smartCounted)
            {
                std::cerr << "[          ] " << smartCounted->getData() << std::endl;
            }
            //smartCounted->suicide();
        }
    }
    factory.inscribe<FactoryType::Singleton, IStringDataFromRoot>([]()
    {
        return new CountedFromRoot("The singleton from root");
    }, "Singleton");
    factory.inscribe<FactoryType::Default, IStringDataFromRoot>([](const std::string & str)
    {
        return new CountedFromRoot(str);
    });
    factory.inscribe<FactoryType::Default, IStringDataFromRoot>([](int number)
    {
        return new CountedFromRoot(number);
    });
    {
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

    typedef RecyclableWrapper<CountedFromRoot> RecyclableCountedFromRoot;
    {
        auto fromPool1(RecyclableCountedFromRoot::createFromPool());
        auto fromPool2(RecyclableCountedFromRoot::createFromPool());
        auto fromPool3(RecyclableCountedFromRoot::createFromPool());
        fromPool1->setData("PooledFromRoot1");
        fromPool2->setData("PooledFromRoot2");
        fromPool3->setData("PooledFromRoot3");
    }
    {
        auto fromPool1(RecyclableCountedFromRoot::createFromPool());
        auto fromPool2(RecyclableCountedFromRoot::createFromPool());
        auto fromPool3(RecyclableCountedFromRoot::createFromPool());
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
        return RecyclableCountedFromRoot::createFromPoolRaw();
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
    RecyclableCountedFromRoot::getPool()->clear();
    std::cerr << "[          ] " << "RecyclableCounted: " << RecyclableCounted::getPool()->avoidedAllocations() << std::endl;
    std::cerr << "[          ] " << "RecyclableCountedFromRoot: " << RecyclableCountedFromRoot::getPool()->avoidedAllocations() << std::endl;
    EXPECT_TRUE(true);
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
