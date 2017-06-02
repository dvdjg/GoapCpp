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

public:
    SignalTest()
    {
    }
protected:
    static void SetUpTestCase()
    {
    }

    static void TearDownTestCase()
    {
    }

    virtual void SetUp() {  }
    virtual void TearDown() {  }
};


TEST_F(SignalTest, Test1)
{
    View l1;
    Controller c;
    c.subscribe(std::bind(&View::OnUpdate, &l1));
    c.subscribe([] { std::cout << "Or we can attach a random action\n"; });

    c.DoWork();
}

