#include <QString>
#include <QtTest>
#include <QDebug>
#include <functional>
#include "factory.h"
#include "irefcounter.h"
#include "refcounter.h"

QDebug operator<< (QDebug d, const std::string &data)
{
    d << data.c_str();
    return d;
}

using namespace goap;

class IStringData : virtual public IRefCounter
{
public:
    virtual const std::string &data() const = 0;
    virtual void setData(const std::string &data) = 0;
};

class IStringDataFromRoot : virtual public IRoot
{
public:
    virtual const std::string &data() const = 0;
    virtual void setData(const std::string &data) = 0;
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
    const std::string &data() const override
    {
        return _data;
    }
    void setData(const std::string &data) override
    {
        _data = data;
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
    const std::string &data() const override
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
        Counted counted("Hola");
        {
            SmartPointerChooser<Counted>::type ptrCounted(new Counted("Hello"));
            {
                auto smartCounted = factory.create<IStringData, const std::string &>({}, "Hallo");
                auto smartCounted2 = factory.create<IStringData, const std::string &>("Lambda", "Haloha");
                auto smartCounted3 = factory.create<IStringData, const std::string &>("Functor", "Jalo");
                auto smartCounted4 = factory.create<IStringData, const std::string &>("Delegate", "Hi");
                if (smartCounted)
                {
                    qInfo() << smartCounted->data();
                }
                //smartCounted->suicide();
            }
        }
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
            if (smartCounted)
            {
                qInfo() << smartCounted->data();
            }
        }
        QVERIFY2(true, "Failure");
    }
};







QTEST_APPLESS_MAIN(FactoryTestTest)

#include "tst_factorytesttest.moc"
