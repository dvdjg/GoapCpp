#include <gmock/gmock.h>
#include <list>
#include "explicit_ptr.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "newptr.h"

using namespace goap;
using namespace std;

class GoapIStateTest : public ::testing::Test
{
public:
    GoapIStateTest() {
    }

protected:
    static void SetUpTestCase() {
    }
    static void TearDownTestCase() {
    }
    virtual void SetUp() {
    }
    virtual void TearDown() {
    }
};

TEST_F(GoapIStateTest, TestSetAt)
{
    auto scopeTimer = NewPtr<IScopeTime>({}, "GoapIStateTest, Test1 A: ", true);
    ASSERT_TRUE(scopeTimer);
    NewPtr<IState> ptrState;
    ASSERT_TRUE(ptrState);
    ptrState->setAt("Uno", NewPtr<IStateValue>({1.f, 2.f}));
    ptrState->setAt("Dos", {1.f, 2.f});

    auto atUno = ptrState->at("Uno");
    auto atDos = ptrState->at("Dos");
    ASSERT_TRUE(atUno->equal(atDos));
    ASSERT_EQ(*atUno, *atDos);

    auto lstTres = {3.f, 6.f};
    auto szTres = "Tres";
    ptrState->setAt(szTres, lstTres);
    auto atTres = ptrState->at("Tres");
    ASSERT_NE(*atUno, *atTres);
    ASSERT_EQ(3, ptrState->size());

    //auto lstValues
    int iTest = 0;
    for (IState::index_type i = 0; i < ptrState->size(); ++i) {
        auto pair = ptrState->at(i);
        std::cout << pair.first->toDebugString() << " : " << pair.second->toDebugString() << std::endl;
        if (*pair.first == szTres) {
            ++iTest;
        }
    }
    ASSERT_EQ(1, iTest);

    //ptrState->resize(10);
    //ptrState->setAt(0, 0.0f);
    //ptrState->setAt(1, 1.0);
    //ptrState->setAt(2, 2.0);
    //ptrState->setAt(3, 9.0);
    //
    //EXPECT_EQ(1.0, ptrState->at(1));
    //EXPECT_EQ(0.0, ptrState->at(10));
    //EXPECT_FLOAT_EQ(1.5, ptrState->at(1.5f)) << "La interpolación no coincide.";
}
