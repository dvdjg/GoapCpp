#include "gtest/gtest.h"
#include <list>
#include <unordered_map>
#include <future>
#include <thread>
#include "termcolor/termcolor.hpp"
#include "explicit_ptr.h"
#include "goaplibrary.h"
#include "basicmath.h"
#include "log_hook.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "backing_a_pie.h"
#include "hanoi_tower_solver.h"
#include "state_change_solver.h"

#include "newptr.h"

using namespace goap;
using namespace std;
using namespace termcolor;

class GoapTest : public ::testing::Test
{
public:
    static const float REF_TEMP;
    launch launch = launch::async; // launch::async launch::deferred
    typedef list<IPlanningAction::CPtr> lst_plan_type;
    GoapTest() {
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

const float GoapTest::REF_TEMP = 300;


TEST_F(GoapTest, TestNumericComparer)
{
    LOG(INFO) <<  magenta << "TestNumericComparer. 01" << reset << endl;
    IState::Ptr initialState = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10} });

    IState::Ptr targetState = NewPtr<IState>()->assign(
            {{"PieIsReadyForEat", true}});

    IState::Ptr state1 = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10}, {"EggIsOnBowl", false} });

    IState::Ptr state2 = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10}, {"EggIsOnBowl", true} });

    IState::Ptr state3 = NewPtr<IState>()->assign(
            { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10}, {"EggIsOnBowl", true}, {"PieIsReadyForEat", true} });

    IState::Ptr backingHelper = NewPtr<IState>()->assign(
            {{"EggIsOnBowl", true}, {"ButterIsOnBowl", true}, {"FlourIsOnBowl", true}, {"IngredientsAreMixed", true}, {"PieIsBaked", true}});

    IPlanningStateComparer::Ptr numericalComparer = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON);

    float distance  = numericalComparer->distance(initialState, backingHelper);
    float distance1 = numericalComparer->distance(state1, backingHelper);
    float distance2 = numericalComparer->distance(state2, backingHelper);
    bool bdistance2 = numericalComparer->enough(state2, targetState);

    EXPECT_GT(distance, distance2);
    EXPECT_GT(distance1, distance2);
    EXPECT_EQ(false, bdistance2);

    float distance3 = numericalComparer->distance(state3, targetState);
    EXPECT_EQ(0, distance3);

    bool bdistance3 = numericalComparer->enough(state3, targetState);
    EXPECT_EQ(true, bdistance3);
}

TEST_F(GoapTest, TestStateSolverOwenIsOn)
{
    state_change_solver solver;
    solver.setInputSequence({"OwenIsOn"});
    solver.setOutputSequence({"OwenIsOn"});
    lst_plan_type plan = solver.makePlan( { {"OwenIsOn", false} }, { {"OwenIsOn", true} } );
    EXPECT_NE(0, plan.size());
    LOG(INFO) << "State Plan OwenIsOn: " << planToOStream(solver.plan(), solver.initialState());
}

TEST_F(GoapTest, TestStateSolverOwenTemperature)
{
    state_change_solver solver;
    solver.setInputSequence({"OwenTemperature"});
    solver.setOutputSequence({"OwenTemperature"});
    lst_plan_type plan = solver.makePlan( { {"OwenTemperature", 300} }, { {"OwenIsOn", true} } );
    EXPECT_NE(0, plan.size());
    LOG(INFO) << "State Plan OwenIsOn: " << planToOStream(solver.plan(), solver.initialState());
}

TEST_F(GoapTest, TestTowerSolver)
{
    future<hanoi_tower_solver> f1 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3} }, { {"C1", 1}, {"C2", 2}, {"C3", 3} }, 3);
        EXPECT_EQ(7, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f2 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4} }, 4);
        EXPECT_EQ(15, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f3 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5} }, 5);
        EXPECT_EQ(31, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f4 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6} }, 6);
        EXPECT_EQ(63, plan.size());
        return tower_solver;
    });

    LOG(INFO) << magenta << "Waiting..." << reset << flush;
    f1.wait();
    f2.wait();
    f3.wait();
    f4.wait();

    auto solver1 = f1.get();
    LOG(INFO) << "Plan: " << planToOStream(solver1.plan(), solver1.initialState());
    auto solver2 = f2.get();
    LOG(INFO) << "Plan: " << planToOStream(solver2.plan(), solver2.initialState());
    auto solver3 = f3.get();
    LOG(INFO) << "Plan: " << planToOStream(solver3.plan(), solver3.initialState());
    auto solver4 = f4.get();
    LOG(INFO) << "Plan: " << planToOStream(solver4.plan(), solver4.initialState());

    LOG(INFO) << magenta << "Computed." << reset << flush;
}

TEST_F(GoapTest, TestTowerSolver255)
{
    future<hanoi_tower_solver> f5 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(127, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f6 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7}, {"A8", 8} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7}, {"C8", 8} }, 8);
        EXPECT_EQ(255, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f7 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lst_plan_type plan = tower_solver.makePlan( { {"A3", 1}, {"A4", 2}, {"A5", 3}, {"A6", 4}, {"A7", 5}, {"B6", 6}, {"B7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(160, plan.size());
        return tower_solver;
    });
    f5.wait();
    f6.wait();
    f7.wait();
    auto solver5 = f5.get();
    LOG(INFO) << "Plan: " << planToOStream(solver5.plan(), solver5.initialState());
    auto solver6 = f6.get();
    LOG(INFO) << "Plan: " << planToOStream(solver6.plan(), solver6.initialState());
    auto solver7 = f7.get();
    LOG(INFO) << "Plan: " << planToOStream(solver7.plan(), solver7.initialState());
}

class StateComparison {
public:
    struct Coefficients {
        float cosDistance = 0, moduleDistance = 0, percInc = 1;
        IStateValue::CPtr srcValue, dstValue;
    };
protected:
    map<IStateValue::CNew, Coefficients> _valueSimilarity;

public:
    IState::New srcCleanState, dstCleanState;
    const map<IStateValue::CNew, Coefficients>& valueSimilarity() { return _valueSimilarity; }

    float compareStates(const IState::CNew& stateSrc, const IState::CNew& stateDst) {
        float percent = 0;
        IState::index_type s2Count = 0;
        IState::index_type sameKeyCount = 0; // Counts the number of keys that stateSrc has like stateDst

        auto itState = stateDst->iterator();
        while (itState->hasNext()) {
            auto pair = itState->next();
            auto key = pair.first;
            Coefficients& coeff = _valueSimilarity[key];
            IStateValue::Ptr valueDst = pair.second;
            IStateValue::Ptr valueSrc;
            if (valueDst) {
                valueSrc = stateSrc->at(key);
                if (valueSrc) {
                    sameKeyCount++;
                    if (valueDst->size() == 1 && valueSrc->size() == 1) {
                        // Si son números únicos, hacer una comparación sencilla
                        float s1k = valueSrc->at(0);
                        float s2k = valueDst->at(0);
                        // 0 < are similar < are different < 1
                        float percInc = basicmath::floatSimilarity(s1k, s2k);
                        float mul = s1k * s2k;
                        coeff.cosDistance = mul > 0 ? 1 : mul < 0 ? -1 : 0;
                        coeff.moduleDistance = 1.f - basicmath::floatSimilarityAbs(s1k, s2k);
                        coeff.percInc = percInc;
                        percent += percInc;
                    } else if (valueDst->size() >= 1 && valueSrc->size() >= 1) {
                        float thisModule, othersModule;
                        float cosDist = valueSrc->cosineDistance(valueDst, &thisModule, &othersModule);
                        float moduleDistance = 1.f - basicmath::floatSimilarity(thisModule, othersModule);
                        float percInc = std::max(0.f, 1.f - cosDist * moduleDistance);
                        coeff.cosDistance = cosDist;
                        coeff.moduleDistance = moduleDistance;
                        coeff.percInc = percInc;
                        percent += percInc;
                    } else {
                        percent += 1.f;
                    }
                    //LOG(DEBUG) << "Compared key=" << *key << ": ValueDst=" << *valueDst << "; ValueSrc=" << *valueSrc << ". AccPercent=" << percent;
                }
            }
            if (coeff.percInc > 0) {
                // There are changes in this value
                coeff.srcValue = valueSrc;
                coeff.dstValue = valueDst;
                if (valueSrc) {
                    srcCleanState->put(key, valueSrc);
                }
                dstCleanState->put(key, valueDst);
            }
            ++s2Count;
        }
        IState::index_type countDifferent = s2Count - sameKeyCount;
        float dist = (percent + countDifferent) / s2Count;
        //LOG(DEBUG) << "NumericDistance=" << dist << "\n  from " << *stateSrc << "\n    to " << *stateDst;
        return dist;
    }
};

TEST_F(GoapTest, TestStateComparison)
{
    IState::New state1 { {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10}, {"Uno", {1,2,0}}, {"Dos", {1,1,0}}, {"JustDisapeared", false}, {"EggIsOnBowl", false}, {"ButterIsOnBowl", false}, {"FlourIsOnBowl", false} };
    IState::New state2 { {"OwenTemperature", REF_TEMP + 3}, {"BowlTemperature", REF_TEMP}, {"Credits", 2}, {"Uno", {5,10,0}}, {"Dos", {0,1,0}}, {"JustCreated", true}, {"EggIsOnBowl", false}, {"ButterIsOnBowl", false}, {"FlourIsOnBowl", false} };
    StateComparison comp;
    float dist1 = comp.compareStates(state1, state2);
    auto &mapSim = comp.valueSimilarity();
    for (auto & it : mapSim) {
        LOG(INFO) << "State:" << *it.first << ",\n    cosDistance:" << it.second.cosDistance << ",    moduleDistance:" << it.second.moduleDistance << ",    percInc:" << it.second.percInc;
    }
    LOG(INFO) << "Clean Src State: " << *comp.srcCleanState;
    LOG(INFO) << "Clean Dst State: " << *comp.dstCleanState;
    ASSERT_EQ(nullptr, comp.srcCleanState->at("EggIsOnBowl"));
    ASSERT_EQ(nullptr, comp.dstCleanState->at("EggIsOnBowl"));

    auto& uno = mapSim.at("Uno");
    ASSERT_FLOAT_EQ(1, uno.cosDistance);
    ASSERT_FLOAT_EQ(0.2f, uno.moduleDistance);
    ASSERT_FLOAT_EQ(0.8f, uno.percInc);
}

TEST_F(GoapTest, TestBackingAPie)
{
    backing_a_pie backing;
    IPlanner::Ptr planner = backing.planner();
    map<string, long long> mapActionAcepted;
    planner->actionStateFunction([&](const IPlanningAction::CPtr &action, const IState::CPtr &initialState, IState::CPtr nextState) {
        if (nextState) {
            ++mapActionAcepted[*action->name()];
        }
    });
    float REF_TEMP = backing_a_pie::REF_TEMP;
    backing.backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    IState::CPtr initialState = backing.initialState();
    list<IPlanningAction::CPtr> plan = backing.MakePlan();

    LOG(INFO) << "Plan: " << planToOStream(plan, initialState);
    EXPECT_LT(4, plan.size());

    for (auto& it : mapActionAcepted) {
        LOG(INFO) << "Action: \"" << it.first << ", Count: " << it.second;
    }
}

