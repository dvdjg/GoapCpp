#include "gtest/gtest.h"
#include <list>
#include <unordered_map>
#include "explicit_ptr.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"
#include "goap/iplanningstatecomparer.h"
#include "goaplibrary.h"

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
        LOG_CONF::singleton().setLevel(DEBUG);
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
    auto it = ptrState->iterator();
    while (it->hasNext()) {
        auto pair = it->next();
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

    IState::Ptr ptrCloned = dynamic_pointer_cast<IState>(ptrState->clone());
    ASSERT_TRUE(ptrCloned);
    ASSERT_EQ(*ptrCloned, *ptrState);
    auto it1 = ptrCloned->at("Dos");
    auto it2 = ptrState->at("Dos");
    ASSERT_EQ(*it1,*it2);

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
}

TEST_F(GoapIStateTest, TestIterate)
{
    map<string, float> mapState;
    NewPtr<IState> ptrState;
    ptrState->assign({ {"Uno", 1}, {"Dos", 2}, {"Tres", 3} });
    NewPtr<IStateValueIterator> itState;
    *itState = ptrState;

    while (itState->hasNext()) {
        auto pair = itState->next();
        mapState[*pair.first] = *pair.second;
        LOG(INFO) << "{" << *pair.first << ": " << *pair.second << "}";
    }
    ASSERT_EQ(2, mapState["Dos"]);
    ASSERT_EQ(3, mapState["Tres"]);
}

TEST_F(GoapIStateTest, TestDistance)
{
    NewPtr<IState> ptrState1, ptrState2, ptrState3, ptrState4, ptrState5;
    ptrState1->assign({ {"Uno", {1,0,0}},  {"Dos", {0,1,0}},  {"Tres", {0,0,1}} });
    ptrState2->assign({ {"Uno", {10,0,0}}, {"Dos", {0,10,0}}, {"Tres", {0,0,10}} });
    ptrState3->assign({ {"Uno", {1,1,0}},  {"Dos", {0,1,1}},  {"Tres", {1,0,1}} });
    ptrState4->assign({ {"Uno", {}},       {"Dos", {0,1,0}},  {"Tres", {0,0,1}} });
    ptrState5->assign({ {"Uno", (IStateValue*)nullptr},       {"Dos", {0,1,0}},  {"Tres", {0,0,1}} });

    IPlanningStateComparer::Ptr numericStateComparer       = NewPtr<IPlanningStateComparer>(string(STR_GOAP_NUMERICSTATECOMPARER_SINGLETON));
    IPlanningStateComparer::Ptr exactStateComparer         = NewPtr<IPlanningStateComparer>(string(STR_GOAP_EXACTSTATECOMPARER_SINGLETON));
    IPlanningStateComparer::Ptr levensteinStateComparer    = NewPtr<IPlanningStateComparer>(string(STR_GOAP_LEVENSTEINSTATECOMPARER_SINGLETON));

    vector<float> lstFlt1;
    for (auto comparer : {numericStateComparer, exactStateComparer, levensteinStateComparer}) {
        float flt1 = comparer->distance(ptrState1, ptrState2);
        float flt2 = comparer->distance(ptrState1, ptrState3);
        float flt3 = comparer->distance(ptrState1, ptrState4);
        float flt4 = comparer->distance(ptrState1, ptrState5);
        lstFlt1.push_back(flt1);
        lstFlt1.push_back(flt2);
        lstFlt1.push_back(flt3);
        lstFlt1.push_back(flt4);
    }

    IPlanningStateMeter::Ptr numericStateMeter      = Goap::newComparerStateMeter(ptrState1, numericStateComparer);
    IPlanningStateMeter::Ptr exactStateMeter        = Goap::newComparerStateMeter(ptrState1, exactStateComparer);
    IPlanningStateMeter::Ptr levensteinStateMeter   = Goap::newComparerStateMeter(ptrState1, levensteinStateComparer);

    vector<float> lstFlt2;
    for (auto comparer : {numericStateMeter, exactStateMeter, levensteinStateMeter}) {
        float flt1 = comparer->distance(ptrState2);
        float flt2 = comparer->distance(ptrState3);
        float flt3 = comparer->distance(ptrState4);
        float flt4 = comparer->distance(ptrState5);
        lstFlt2.push_back(flt1);
        lstFlt2.push_back(flt2);
        lstFlt2.push_back(flt3);
        lstFlt2.push_back(flt4);
    }

    ASSERT_EQ(lstFlt1, lstFlt2);
}
