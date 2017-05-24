#include <QString>
#include <QtTest>
#include <QDebug>
#include <functional>
#include "factory.h"
#include "irefcounter.h"
#include "refcounter.h"

QDebug operator<< (QDebug d, const std::string& data)
{
    d << data.c_str();
    return d;
}

using namespace goap;

class IStringData : virtual public IRefCounter
{
public:
    virtual const std::string& data() const = 0;
    virtual void setData(const std::string& data) = 0;
};

class Counted : virtual public IStringData
{
    IMPLEMENT_REFCOUNTER()

    std::string _data;
public:
    typedef boost::intrusive_ptr<Counted> smart_pointer;
    Counted(const std::string& data = {}) : _data(data)
    {
        qInfo() << "Created " << _data;
    }
    ~Counted()
    {
        qInfo() << "Destroyed " << _data;
    }
    const std::string& data() const override;
    void setData(const std::string& data) override;
};

Counted* createCounted(const std::string& str) { return new Counted(str); }

class FactoryTestTest : public QObject
{
    Q_OBJECT

public:
    FactoryTestTest()
    {
    }

private Q_SLOTS:
    void initTestCase()
    { qDebug("called before"); }
    void init()
    { QVERIFY(1 == 1); }
    void cleanup()
    { QVERIFY(1 != 2); }
    void cleanupTestCase()
    { qDebug("called afte"); }

    void testCase1()
    {
        auto& factory = Factory<IRoot>::singleton();

        //auto l = []() {return new Counted;};
        auto lstr = [](const std::string & str) {return new Counted(str);};
        std::function<Counted* (const std::string& str)> f = lstr;
        //Factory<IRoot>::singleton().Register<IStringData, Counted>([](int i) { return new Counted("Dentro");});
        factory.inscribe<IStringData>(&createCounted);
        factory.inscribe<IStringData>(f, "Functor");
        factory.inscribe<IStringData>(lstr, "Lambda");
        factory.inscribe<IStringData, Counted, const std::string&>("Delegate");
        Counted counted("Hola");
        {
            Counted::smart_pointer ptrCounted(new Counted("Hello"));
            {
                auto smartCounted = factory.create<IStringData, const std::string&>({}, "Hallo");
                auto smartCounted2 = factory.create<IStringData, const std::string&>("Lambda", "Haloha");
                auto smartCounted3 = factory.create<IStringData, const std::string&>("Functor", "Jalo");
                auto smartCounted4 = factory.create<IStringData, const std::string&>("Delegate", "Hi");
                if (smartCounted)
                    qInfo() << smartCounted->data();
                //smartCounted->suicide();
            }
        }
        QVERIFY2(true, "Failure");
    }
};



const std::string& Counted::data() const
{
    return _data;
}

void Counted::setData(const std::string& data)
{
    _data = data;
}

QTEST_APPLESS_MAIN(FactoryTestTest)

#include "tst_factorytesttest.moc"
