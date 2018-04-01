#include <gmock/gmock.h>
#include <boost/signals2.hpp>
#include <iostream>

struct View
{
    void OnUpdate()
    {
        std::cout << "Updating in View\n";
    }
};

class Controller
{
    using UpdateHandler = boost::signals2::signal<void()>;
    UpdateHandler sig;

public:
    Controller() {}

    void subscribe(UpdateHandler::slot_type handler)
    {
        sig.connect(handler);
    }
    void DoWork() const
    {
        sig();
    }
    void Update();
};


class SignalTest : public ::testing::Test
{
    int _counter = 0;
public:
    SignalTest()
    {
    }

    void OnUpdate()
    {
        std::cout << "Updating in SignalTest\n";
        ++_counter;
    }
protected:

    int counter() const
    {
        return _counter;
    }

    void setCounter(int counter)
    {
        _counter = counter;
    }

    static void SetUpTestCase()
    {
    }

    static void TearDownTestCase()
    {
    }

    virtual void SetUp() { _counter = 0; }
    virtual void TearDown() {  }
};


TEST_F(SignalTest, Test1)
{
    View l1;
    Controller c;
    c.subscribe(std::bind(&View::OnUpdate, &l1));
    c.subscribe(std::bind(&SignalTest::OnUpdate, this));
    c.subscribe([&] { std::cout << "Or we can attach a random action\n"; setCounter(counter()+1);});

    c.DoWork();

    EXPECT_EQ(2, counter());
}



