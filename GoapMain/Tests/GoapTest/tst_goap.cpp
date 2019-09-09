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
    auto scopeTimer = NewPtr<IScopeTime>({}, "GoapIStateTest, TestSetAt: ", true);
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
    auto atTres = ptrState->at(szTres);
    ASSERT_NE(*atUno, *atTres);
    ASSERT_EQ(3, ptrState->size());

    int iTest = 0;
    for (IState::index_type i = 0; i < ptrState->size(); ++i) {
        auto pair = ptrState->at(i);
        std::cout << pair.first->toDebugString() << " : " << pair.second->toDebugString() << std::endl;
        if (*pair.first == szTres && *pair.second == lstTres) {
            ++iTest;
        }
    }
    ASSERT_EQ(1, iTest);

    ptrState->remove(szTres);
    iTest = 0;
    for (IState::index_type i = 0; i < ptrState->size(); ++i) {
        auto pair = ptrState->at(i);
        std::cout << pair.first->toDebugString() << " : " << pair.second->toDebugString() << std::endl;
        if (*pair.first == szTres && *pair.second == lstTres) {
            ++iTest;
        }
    }
    ASSERT_EQ(0, iTest);
}


TEST_F(GoapIStateTest, TestClone)
{
    NewPtr<IState> ptrState;
    ASSERT_TRUE(ptrState);
    ptrState->setCost(23);
    ptrState->setAt("Uno", {1, 2});
    ptrState->setAt("Dos", {6.6f, 2.4f, 9});
    //std::shared_ptr<IClonable> shCl;
    //auto ptr = std::dynamic_pointer_cast<IState>(shCl);

    auto ptrCloned = dynamic_pointer_cast<IState>(ptrState->clone());
    ASSERT_TRUE(ptrCloned);
    ASSERT_EQ(*ptrCloned, *ptrState);
    ASSERT_EQ(ptrCloned->at(0), ptrState->at(0));

    NewPtr<IState> ptrOther(ptrState->clone());

    ASSERT_EQ(*ptrOther, *ptrState);
}
