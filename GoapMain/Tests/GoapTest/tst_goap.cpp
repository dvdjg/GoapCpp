#include <gmock/gmock.h>
#include <list>
#include <unordered_map>
#include "explicit_ptr.h"

#include "goap/istate.h"
#include "goap/iscopetimer.h"

#include "backing_a_pie.h"

#include "newptr.h"

using namespace goap;
using namespace std;

class GoapTest : public ::testing::Test
{
public:
    GoapTest() {
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

TEST_F(GoapTest, TestBackingAPie)
{
    backing_a_pie backing;
    //backing.run();
    float REF_TEMP = backing_a_pie::REF_TEMP;
    backing.backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    IState::CPtr initialState = backing.initialState();
    std::list<IPlanningAction::CPtr> plan = backing.MakePlan();

    LOG(INFO) << "Plan:\n" << IPlanningAction::planToString(plan, initialState);
    EXPECT_GT(5, plan.size());
}

