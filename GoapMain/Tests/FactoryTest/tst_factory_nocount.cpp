#include <gtest/gtest.h>

#include "iroot.h"
#include "factory.h"
#include "reuseobjectpool.h"


using namespace goap;
namespace goap_noboost
{

class IStringDataFromRoot : virtual public IRoot
{
public:
    virtual const std::string &getData() const = 0;
    virtual void setData(const std::string &getData) = 0;
};

class NonCounted : virtual public IStringDataFromRoot
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



class FactoryNoCountTest : public ::testing::Test
{

public:
    FactoryNoCountTest()
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

using namespace goap_noboost;

TEST_F(FactoryNoCountTest, Test1)
{
    auto &factory = *Factory<IRoot>::singleton();

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

    factory.inscribe<FactoryType::Default, IStringDataFromRoot, NonCounted, const std::string &>("Delegate");

    factory.inscribe<FactoryType::Default, IStringDataFromRoot, NonCounted, int>("Delegate2");

    {
        auto smartCounted = factory.create<IStringDataFromRoot, const std::string &>({}, "Bye");
        auto smartCounted2 = factory.create<IStringDataFromRoot>({}, 123);
        auto smartCounted3 = factory.create<IStringDataFromRoot>("Singleton");
        auto smartCounted4 = factory.create<IStringDataFromRoot>("Singleton");
        auto smartCounted5 = factory.create<IStringDataFromRoot, const std::string &>("Delegate", "Hi");
        auto smartCounted6 = factory.create<IStringDataFromRoot>("Delegate2", 12345678);
        if (smartCounted)
        {
            std::cerr << "[          ] " << smartCounted->getData() << std::endl;
        }
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
    RecyclableNonCounted::getPool()->clear();
    std::cerr << "[          ] " << "RecyclableNonCounted: " << RecyclableNonCounted::getPool()->avoidedAllocations() << std::endl;
    EXPECT_TRUE(true);
}



