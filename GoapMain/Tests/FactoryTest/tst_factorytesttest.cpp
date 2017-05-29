#include <gmock/gmock.h>

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

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
#include <QString>
#include <QtTest>
#include <QDebug>
#include <functional>
#include "factory.h"
#include "irefcounter.h"
#include "refcounter.h"
#include "reuseobjectpool.h"

QDebug operator<< (QDebug d, const std::string &data)
{
    d << data.c_str();
    return d;
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
        qInfo() << "Created " << _data;
    }
    CountedFromRoot(int number) : _data(std::to_string(number))
    {
        qInfo() << "Created " << _data;
    }
    ~CountedFromRoot()
    {
        qInfo() << "Destroyed " << _data;
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
        qInfo() << "Created " << _data;
    }
    Counted(int number) : _data(std::to_string(number))
    {
        qInfo() << "Created " << _data;
    }
    ~Counted()
    {
        qInfo() << "Destroyed " << _data;
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


class FactoryTestTest : public QObject
{
    Q_OBJECT

public:
    FactoryTestTest()
    {
    }

private Q_SLOTS:
    void initTestCase()
    {
        qDebug("initTestCase()");
    }
    void init()
    {
        QVERIFY(1 == 1);
    }
    void cleanup()
    {
        QVERIFY(1 != 2);
    }
    void cleanupTestCase()
    {
        qDebug("cleanupTestCase()");
    }

    void testCase1()
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
                    qInfo() << smartCounted->getData();
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
                qInfo() << smartCounted->getData();
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
            qInfo() << fromPool1->getData();
            qInfo() << fromPool2->getData();
            qInfo() << fromPool3->getData();
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
            qInfo() << fromPool1->getData();
            qInfo() << fromPool2->getData();
            qInfo() << fromPool3->getData();
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
        qInfo() << "Cleanup";
        RecyclableCounted::getPool()->clear();
        RecyclableCountedFromRoot::getPool()->clear();
        qInfo() << "RecyclableCounted: " << RecyclableCounted::getPool()->avoidedAllocations();
        qInfo() << "RecyclableCountedFromRoot: " << RecyclableCountedFromRoot::getPool()->avoidedAllocations();
        QVERIFY2(true, "Failure");
    }
};


QTEST_APPLESS_MAIN(FactoryTestTest)

#include "tst_factorytesttest.moc"
*/
