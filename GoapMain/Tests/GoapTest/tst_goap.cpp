#include <gmock/gmock.h>
#include <list>
#include <unordered_map>
#include <future>
#include <thread>
#include "termcolor/termcolor.hpp"
#include "explicit_ptr.h"
#include "goaplibrary.h"

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
    std::cout << magenta << "TestNumericComparer. 01" << reset << std::endl;
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
    typedef list<IPlanningAction::CPtr> lstPlan;
    auto launch = std::launch::async; // std::launch::async std::launch::deferred
    std::future<lstPlan> f1 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3} }, { {"C1", 1}, {"C2", 2}, {"C3", 3} }, 3);
        EXPECT_EQ(7, plan.size());
        return plan;
    });

    f1.wait();
    std::future<lstPlan> f2 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4} }, 4);
        EXPECT_EQ(15, plan.size());
        return plan;
    });
    std::future<lstPlan> f3 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5} }, 5);
        EXPECT_EQ(31, plan.size());
        return plan;
    });
    std::future<lstPlan> f4 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6} }, 6);
        EXPECT_EQ(63, plan.size());
        return plan;
    });
    std::future<lstPlan> f5 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(127, plan.size());
        return plan;
    });
    std::future<lstPlan> f6 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7}, {"A8", 8} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7}, {"C8", 8} }, 8);
        EXPECT_EQ(255, plan.size());
        return plan;
    });
    std::future<lstPlan> f7 = std::async(launch, []{
        hanoi_tower_solver tower_solver;
        lstPlan plan = tower_solver.makePlan( { {"A3", 1}, {"A4", 2}, {"A5", 3}, {"A6", 4}, {"A7", 5}, {"B6", 6}, {"B7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
        EXPECT_EQ(160, plan.size());
        return plan;
    });
    std::cout << magenta << "Waiting..." << reset << std::flush;
    f2.wait();
    f3.wait();
    f4.wait();
    f5.wait();
    f6.wait();
    f7.wait();
    std::cout << magenta << "Computed." << reset << std::flush;
}

TEST_F(GoapTest, TestBackingAPie)
{
    backing_a_pie backing;
    //backing.run();
    float REF_TEMP = backing_a_pie::REF_TEMP;
    backing.backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    IState::CPtr initialState = backing.initialState();
    std::list<IPlanningAction::CPtr> plan = backing.MakePlan();

    LOG(INFO) << "Plan:";
    IPlanningAction::planToOstream(cerr, plan, initialState);
    EXPECT_LT(4, plan.size());
}

