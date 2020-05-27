#ifndef BACKING_A_PIE_H
#define BACKING_A_PIE_H

#include <list>
#include "goap/iplanner.h"
#include "goap/istate.h"
#include "goap/iscopetimer.h"
#include "goap/ifunctionstatemeter.h"
#include "goap/iplanningstatecomparer.h"
#include "newptr.h"
#include "log_hook.h"

using namespace goap;

/**
 * Plan Formulation
 *
 * A character generates a plan in real-time by supplying some goal to satisfy to a system
 * called a planner. The planner searches the space of actions for a sequence that will take
 * the character from his starting state to his goal state. This process is referred to as
 * formulatinga plan. If the planner is successful, it returns a plan for the character to follow
 * to direct his behavior. The character follows this plan to completion, invalidation, or until
 * another goal becomes more relevant. If another goal activates, or the plan in-progress
 * becomes invalid for any reason, the character aborts the current plan and formulates a
 * new one.
 */
class backing_a_pie
{
    IPlanner::Ptr _planner;
    IState::CPtr _initialState;
    IState::CPtr _goalState;

    IState::Ptr _backingHelper; // = State.getNew({EggIsOnBowl:true, ButterIsOnBowl:true, FlourIsOnBowl:true, IngredientsAreMixed:true, PieIsBaked:true});
    IState::Ptr _restHelper; // = State.getNew({EggIsOnBowl:true, ButterIsOnBowl:true, FlourIsOnBowl:true, IngredientsAreMixed:true, PieIsBaked:true});
    IState::Ptr _orderHelper; // = State.getNew({PieIsComing:5});
    IPlanningStateMeter::CPtr _planningStateMeter;

    static IState::Ptr wait(IState::Ptr state);

    /**
        Baking a pie
        ============
        If the overall goal is to bake a pie the end-state could be
        (PieIsBaked, true). The plan is then formulated based on the actions
        available and their costs, which for instance might include TurnOnOwen,
        PutBowlOnTable, AddEggToBowl, AddButterToBowl, AddFlourToBowl,
        MixBowlIngredients, PutBowlInOwen, Wait, and PutBowlOnTable. In this
        case the first action would be TurnOnOwen which has no preconditions, just as
        PutBowlOnTable. However the actions AddEggToBowl, AddButterToBowl
        and AddFlourToBowl might have the precondition (BowlIsOnTable, true)
        which is the very effect of the action PutBowlOnTable. If the mixture is
        supposed to fare better if the egg is used before that butter, which in turn is
        used before the flour; then AddEggToBowl is given the lowest cost value of the
        three actions whilst AddButter is given a lower cost value than
        AddFlourToBowl. The baking is then allowed to continue by utilizing the
        remaining actions of the plan, resulting in a baked pie once the final action
        PutBowlOnTable has been executed.
     */
    static IPlanner::Ptr backing_actions();

public:
    static const float REF_TEMP;

    backing_a_pie();

    void backing_plan(IState::map_value2value_type initial, IState::map_value2value_type goal);

    void run();

    std::list<IPlanningAction::CPtr> MakePlan();

    IState::CPtr initialState() const;
    IPlanner::Ptr planner() const;
};

#endif // BACKING_A_PIE_H
