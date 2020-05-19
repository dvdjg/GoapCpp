#include <gmock/gmock.h>
#include <list>
#include <unordered_map>
#include "explicit_ptr.h"
#include "goaplibrary.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "backing_a_pie.h"

#include "newptr.h"

using namespace goap;
using namespace std;

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

TEST_F(GoapTest, TestBackingAPie)
{
    backing_a_pie backing;
    //backing.run();
    float REF_TEMP = backing_a_pie::REF_TEMP;
    backing.backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    IState::CPtr initialState = backing.initialState();
    std::list<IPlanningAction::CPtr> plan = backing.MakePlan();

    LOG(INFO) << "Plan:\n";
    IPlanningAction::planToOstream(cerr, plan, initialState);
    EXPECT_GT(5, plan.size());
}

