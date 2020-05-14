#include <gmock/gmock.h>
#include <list>
#include <unordered_map>
#include "explicit_ptr.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "newptr.h"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wno-global-constructors"

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
    ptrState->put("Uno", NewPtr<IStateValue>({1.f, 2.f}));
    ptrState->put("Dos", {1.f, 2.f});

    auto atUno = ptrState->at("Uno");
    auto atDos = ptrState->at("Dos");
    ASSERT_TRUE(atUno->equals(atDos));
    ASSERT_EQ(*atUno, *atDos);

    auto lstTres = {3.f, 6.f};
    auto szTres = "Tres";
    ptrState->put(szTres, lstTres);
    auto atTres = ptrState->at(szTres);
    ASSERT_NE(*atUno, *atTres);
    ASSERT_EQ(3, ptrState->size());

    int iTest = 0;
    for (IState::index_type i = 0; i < ptrState->size(); ++i) {
        auto pair = ptrState->at(i);
        std::cout << pair.first->toDebugString() << " : " << pair.second->toDebugString() << std::endl;
        auto &f = *pair.first;
        auto &s = *pair.second;
        if (f == szTres
                && s == lstTres) {
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
    ptrState->cost(23);
    ptrState->put("Uno", {1, 2});
    ptrState->put("Dos", {6.6f, 2.4f, 9});
    //std::shared_ptr<IClonable> shCl;
    //auto ptr = std::dynamic_pointer_cast<IState>(shCl);

    auto ptrCloned = dynamic_pointer_cast<IState>(ptrState->clone());
    ASSERT_TRUE(ptrCloned);
    ASSERT_EQ(*ptrCloned, *ptrState);
    ASSERT_EQ(ptrCloned->at(0), ptrState->at(0));

    NewPtr<IState> ptrOther(ptrState->clone());

    ASSERT_EQ(*ptrOther, *ptrState);

    std::cout << ptrOther->toString();
    std::cout << ptrOther->toDebugString();
}

TEST_F(GoapIStateTest, TestCreate)
{
    typedef std::unordered_map<IStateValue::CPtr, IStateValue::Ptr> data_type;
    std::initializer_list<data_type::value_type> dt({{NewPtr<IStateValue>({}, "Uno"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})},
                  {NewPtr<IStateValue>({}, "Dos"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})}});
    NewPtr<IState> ptrState(dt);

    std::cout << ptrState->toDebugString();
    //NewPtr<IState> ptrState({{NewPtr<IStateValue>({}, "Uno"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})},
    //          {NewPtr<IStateValue>({}, "Dos"),NewPtr<IStateValue>({1.f, 0.5f, 0.f, 9.f, 98.f})}});
}

