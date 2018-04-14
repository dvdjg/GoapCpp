#include <gtest/gtest.h>

#include "common/iroot.h"
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
    FactoryNoCountTest() {
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
    Factory<IRoot> factory;

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

    factory.inscribe<FactoryType::Default, IStringDataFromRoot, NonCounted, int>("Delegate");

    {
        auto smartCounted1 = factory.create<IStringDataFromRoot, const std::string &>({}, "Bye");
        if (smartCounted1) std::cerr << "[         1] " << smartCounted1->getData() << std::endl;
        auto smartCounted2 = factory.create<IStringDataFromRoot>({}, 123);
        if (smartCounted2) std::cerr << "[         2] " << smartCounted2->getData() << std::endl;
        auto smartCounted3 = factory.create<IStringDataFromRoot>("Singleton");
        if (smartCounted3) std::cerr << "[         3] " << smartCounted3->getData() << std::endl;
        auto smartCounted4 = factory.create<IStringDataFromRoot>("Singleton");
        if (smartCounted4) std::cerr << "[         4] " << smartCounted4->getData() << std::endl;
        auto smartCounted5 = factory.create<IStringDataFromRoot, const std::string &>("Delegate", "Hi");
        if (smartCounted5) std::cerr << "[         5] " << smartCounted5->getData() << std::endl;
        auto smartCounted6 = factory.create<IStringDataFromRoot>("Delegate", 12345678);
        if (smartCounted6) std::cerr << "[         6] " << smartCounted6->getData() << std::endl;
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
        std::cerr << "[        p1] " << fromPool1->getData() << std::endl;
        std::cerr << "[        p2] " << fromPool2->getData() << std::endl;
        std::cerr << "[        p3] " << fromPool3->getData() << std::endl;
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
    auto createdMap = factory.createAll<IStringDataFromRoot, const std::string &>("Datos");
    for (auto created : createdMap) {
        std::cerr << "[       Map] " << created.first << " -> " << created.second->getData() << std::endl;
    }
    std::cerr << "[          ] " << "Cleanup" << std::endl;
    RecyclableNonCounted::getPool()->clear();
    std::cerr << "[          ] " << "RecyclableNonCounted: " << RecyclableNonCounted::getPool()->avoidedAllocations() << std::endl;
    EXPECT_TRUE(true);
}



