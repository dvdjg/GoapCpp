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
