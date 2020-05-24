#include "gtest/gtest.h"
#include <list>
#include <unordered_map>
#include <future>
#include <thread>
#include "termcolor/termcolor.hpp"
#include "explicit_ptr.h"
#include "goaplibrary.h"
#include "log_hook.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "backing_a_pie.h"
#include "hanoi_tower_solver.h"

#include "newptr.h"

using namespace goap;
using namespace std;
using namespace termcolor;

class GoapTest : public ::testing::Test
{
public:
    static const float REF_TEMP;
    launch launch = launch::async; // launch::async launch::deferred
    typedef list<IPlanningAction::CPtr> lstPlan;
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

TEST_F(GoapTest, TestTowerSolver)
{
    future<hanoi_tower_solver> f1 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3} }, { {"C1", 1}, {"C2", 2}, {"C3", 3} }, 3);
        EXPECT_EQ(7, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f2 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4} }, 4);
        EXPECT_EQ(15, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f3 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5} }, 5);
        EXPECT_EQ(31, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f4 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6} }, 6);
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
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(127, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f6 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7}, {"A8", 8} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7}, {"C8", 8} }, 8);
        EXPECT_EQ(255, plan.size());
        return tower_solver;
    });
    future<hanoi_tower_solver> f7 = async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A3", 1}, {"A4", 2}, {"A5", 3}, {"A6", 4}, {"A7", 5}, {"B6", 6}, {"B7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
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

TEST_F(GoapTest, TestBackingAPie)
{
    backing_a_pie backing;
    float REF_TEMP = backing_a_pie::REF_TEMP;
    backing.backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    IState::CPtr initialState = backing.initialState();
    list<IPlanningAction::CPtr> plan = backing.MakePlan();

    LOG(INFO) << "Plan: " << planToOStream(plan, initialState);
    EXPECT_LT(4, plan.size());
}

