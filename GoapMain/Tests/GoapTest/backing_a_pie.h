#ifndef BACKING_A_PIE_H
#define BACKING_A_PIE_H

#include <list>
#include "goap/iplanner.h"
#include "goap/istate.h"
#include "goap/iscopetimer.h"
#include "functionstatemeter.h"
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
    static float REF_TEMP;
    IPlanner::Ptr _planner;
    IState::CPtr _initialState;
    IState::CPtr _goalState;

public:
    backing_a_pie();

    void backing_plan(IState::map_string_float_type initial, IState::map_string_float_type goal)
    {
        _initialState = NewPtr<IState> ({}, initial);
        _goalState = NewPtr<IState> ({}, goal);
    }

    void run()
    {
        //std::list<IPlanningAction::Ptr> plan; // IEnumerable<IPlanningAction<State>>
        backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} },
            {{"PieIsReadyForEat", 1}});
        std::list<IPlanningAction::CPtr> plan = MakePlan();

        LOG(INFO) << "Plan:\n" << goap::planToString(plan, _initialState);
    }

private:
    static IState::Ptr wait(IState::Ptr state)
    {
        float OwenTemperature = state->at("OwenTemperature");
        if (state->at("OwenIsOn") == 1) {
            if (OwenTemperature < REF_TEMP + 200) {
                OwenTemperature += 40;
            }
        } else if (OwenTemperature > REF_TEMP) {
            OwenTemperature -= 20;
        }
        float BowlTemperature = state->at("BowlTemperature");
        if (state->at("BowlLocation") == "Owen") {
            if (OwenTemperature > BowlTemperature + 20) {
                BowlTemperature += 20;
            } else if (OwenTemperature <= BowlTemperature - 10) {
                BowlTemperature -= 10;
            }
        }else if (BowlTemperature > REF_TEMP) {
            BowlTemperature -= 20;
        }

        state->put("BowlTemperature", BowlTemperature);
        state->put("OwenTemperature", OwenTemperature);

        if (state->at("PieIsComing") > 0) {
            state->add("PieIsComing", 1);
        }
        return state;
    }



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
    void backing_actions()
    {
        std::list<IPlanningAction::Ptr> planningActions {
            NewPtr<IPlanningAction>({},
                "TurnOnOwen",
                [](IState::CPtr state) -> bool { return state->at("OwenIsOn") != 1; },
                [](IState::Ptr  state) -> void { wait(state->put("OwenIsOn", 1));  }),
            NewPtr<IPlanningAction>({},
                "TurnOffOwen",
                [](IState::CPtr state) -> bool { return state->at("OwenIsOn") == true; },
                [](IState::Ptr  state) -> void { wait(state->put("OwenIsOn", false)); }),
            NewPtr<IPlanningAction>({},
                "PutBowlOnTable",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") != "Table"; },
                [](IState::Ptr  state) -> void { wait(state->put("BowlLocation", "Table")); }),
            NewPtr<IPlanningAction>({},
                "AddEggToBowl",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") == "Table" && state->at("EggIsOnBowl") != true; },
                [](IState::Ptr  state) -> void { wait(state->put("EggIsOnBowl", true)->mulCost(state->at("ButterIsOnBowl") != true ? 0.9 : 1.0)); }),
            NewPtr<IPlanningAction>({},
                "AddButterToBowl",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") == "Table" && state->at("ButterIsOnBowl") != true; },
                [](IState::Ptr  state) -> void { wait(state->put("ButterIsOnBowl", true)->mulCost(state->at("FlourIsOnBowl") != true ? 0.9 : 1.0)); }),
            NewPtr<IPlanningAction>({},
                "AddFlourToBowl",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") == "Table" && state->at("FlourIsOnBowl") != true; },
                [](IState::Ptr  state) -> void { wait(state->put("FlourIsOnBowl", true)); }),
            NewPtr<IPlanningAction>({},
                "MixBowlIngredients",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") == "Table" && /*state->at("IngredientsAreMixed") != true &&*/ state->at("EggIsOnBowl") == true && state->at("ButterIsOnBowl") == true && state->at("FlourIsOnBowl") == true; },
                [](IState::Ptr  state) -> void { wait(state->put("IngredientsAreMixed", true)); }),
            NewPtr<IPlanningAction>({},
                "PutBowlInOwen",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") != "Owen" && state->at("IngredientsAreMixed") == true && state->at("OwenTemperature") >= (REF_TEMP+100); },
                [](IState::Ptr  state) -> void { wait(state->put("BowlLocation", "Owen")); }),
            NewPtr<IPlanningAction>({},
                "BakePie",
                [](IState::CPtr state) -> bool { return state->at("BowlLocation") == "Owen" && /*state->at("PieIsBaked") != true &&*/ state->at("BowlTemperature") >= (REF_TEMP+100); },
                [](IState::Ptr  state) -> void { wait(state->put("PieIsBaked", true)); }),
            NewPtr<IPlanningAction>({},
                "PieReadyForEat",
                [](IState::CPtr state) -> bool { return state->at("PieIsBaked") == true && state->at("BowlLocation") == "Table" && state->at("BowlTemperature") < (REF_TEMP+30); },
                [](IState::Ptr  state) -> void { wait(state->put("PieIsReadyForEat", true)); }),
            NewPtr<IPlanningAction>({},
                "OrderPie",
                [](IState::CPtr state) -> bool { return state->at("Credits") >= 8; },
                [](IState::Ptr  state) -> void { wait(state->put("PieIsComing", 1)->add("Credits", -8)->mulCost(13)); }),
            NewPtr<IPlanningAction>({},
                "ReceivePie",
                [](IState::CPtr state) -> bool { return state->at("PieIsComing") >= 5; },
                [](IState::Ptr  state) -> void { wait(state->put("PieIsReadyForEat", true)->put("PieIsComing", 0)); }),
            NewPtr<IPlanningAction>({},
                "Wait",
                [](IState::CPtr state) -> bool { return true; },
                [](IState::Ptr  state) -> void { wait(state->mulCost(0.9)); } )
        };
        _planner = NewPtr<IPlanner>({}, IPlanner::BreadthFirst, planningActions);
    }

    IState::Ptr _backingHelper; // = State.getNew({EggIsOnBowl:true, ButterIsOnBowl:true, FlourIsOnBowl:true, IngredientsAreMixed:true, PieIsBaked:true});
    IState::Ptr _orderHelper; // = State.getNew({PieIsComing:5});
    IPlanningStateMeter::CPtr _planningStateMeter;
public:
    std::list<IPlanningAction::CPtr> MakePlan()
    {
        if (!_planningStateMeter || !_planningStateMeter->goalState()->equals(_goalState)) {
            auto functionStateMeter = NewPtr<FunctionStateMeter>({}, _goalState);
            functionStateMeter->fnDistance([=](IState::CPtr state, FunctionStateMeter::CPtr stateMeter)
            {
                float ret = 1;
                ret = stateMeter->numericStateMeter()->distance(state); // Distance to goal
                float distance = ret;
                if (state->at("PieIsComing") == 0) {
                    // A conditional suggestion
                    distance = NewPtr<IPlanningStateComparer>({STR_GOAP_NUMERICSTATECOMPARER_SINGLETON})->distance(state, _backingHelper) * 0.8 + 0.2;
                    //distance = NumericStateComparer::singleton.distance(state, _backingHelper) * 0.8 + 0.2;
                }
                if (state->at("OwenTemperature") == REF_TEMP ) {
                    // A conditional suggestion
                    distance = NewPtr<IPlanningStateComparer>({STR_GOAP_NUMERICSTATECOMPARER_SINGLETON})->distance(state, _orderHelper) * 0.8 + 0.2;
                }
                if (ret > distance)
                    ret = distance;
                return ret;
            });
            _planningStateMeter = functionStateMeter;
        }

        std::list<IPlanningAction::CPtr> actionsArray;
        auto scopeTimer = NewPtr<IScopeTime>({}, "MakePlan: ", true);
        actionsArray = _planner->makePlanCached(_initialState, _planningStateMeter, actionsArray);
        //trace("[Time]", actionsArray.length, "actions,", (t2-t1) + "ms")
        return actionsArray;
    }

};

#endif // BACKING_A_PIE_H
