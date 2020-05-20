#include "backing_a_pie.h"
#include "newptr.h"
#include "goaplibrary.h"

const float backing_a_pie::REF_TEMP = 300; // Kelvin

using namespace goap;
using namespace std;

backing_a_pie::backing_a_pie() {
    _backingHelper = NewPtr<IState>()->assign(
        {{"EggIsOnBowl", true}, {"ButterIsOnBowl", true}, {"FlourIsOnBowl", true}, {"IngredientsAreMixed", true}, {"PieIsBaked", true}});
    _restHelper = NewPtr<IState>()->assign(
        {{"PieIsBaked", true}, {"BowlLocation", "Table"}, {"BowlTemperature",REF_TEMP}});
    _orderHelper = NewPtr<IState>()->assign({{"PieIsComing", 5}});

    _planner = backing_actions();
    LOG(DEBUG) << "Planner backed actions:\n" << *_planner;
}

void backing_a_pie::backing_plan(IState::map_value2value_type initial, IState::map_value2value_type goal) {
    _initialState = NewPtr<IState>()->assign(initial);
    _goalState = NewPtr<IState>()->assign(goal);
}

void backing_a_pie::run() {
    backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP},  {"OwenIsOn", false}, {"Credits", 10} }, {{"PieIsReadyForEat", true}});
    std::list<IPlanningAction::CPtr> plan = MakePlan();

    LOG(INFO) << "Plan actions:\n" << IPlanningAction::planToString(plan, _initialState);
}

IState::CPtr backing_a_pie::initialState() const {
    return _initialState;
}

IState::Ptr backing_a_pie::wait(IState::Ptr state) {
    //LOG(DEBUG) << "Before Waiting:\n " << *state;
    float OwenTemperature = state->atRef("OwenTemperature");
    if (state->atRef("OwenIsOn") == true) {
        if (OwenTemperature < REF_TEMP + 200) {
            OwenTemperature += 40;
        }
    } else if (OwenTemperature > REF_TEMP) {
        OwenTemperature -= 20;
    }
    float BowlTemperature = state->atRef("BowlTemperature");
    if (state->atRef("BowlLocation") == "Owen") {
        if (OwenTemperature > BowlTemperature + 20) {
            BowlTemperature += 20;
        } else if (OwenTemperature <= BowlTemperature - 10) {
            BowlTemperature -= 10;
        }
    } else if (BowlTemperature > REF_TEMP) {
        BowlTemperature -= 10;
    }

    state->put("BowlTemperature", BowlTemperature);
    state->put("OwenTemperature", OwenTemperature);

    if (state->atRef("PieIsComing") > 0) {
        state->add("PieIsComing", 1);
    }
    //LOG(DEBUG) << "After Waiting:\n " << *state;
    return state;
}

IPlanner::Ptr backing_a_pie::backing_actions() {
    std::list<IPlanningAction::CPtr> planningActions {
        Goap::newPlanningAction("TurnOnOwen",
                                [](IState::CPtr state) -> bool { return state->atRef("OwenIsOn") != true; },
                                [](IState::Ptr  state) -> void { wait(state->put("OwenIsOn", true));  }),
        Goap::newPlanningAction("TurnOffOwen",
                                [](IState::CPtr state) -> bool { return state->atRef("OwenIsOn") == true; },
                                [](IState::Ptr  state) -> void { wait(state->put("OwenIsOn", false)); }),
        Goap::newPlanningAction("PutBowlOnTable",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") != "Table"; },
                                [](IState::Ptr  state) -> void { wait(state->put("BowlLocation", "Table")); }),
        Goap::newPlanningAction("AddEggToBowl",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("EggIsOnBowl") != true; },
                                [](IState::Ptr  state) -> void { wait(state->put("EggIsOnBowl", true)->mulCost(state->atRef("ButterIsOnBowl") != true ? 0.9 : 1.0)); }),
        Goap::newPlanningAction("AddButterToBowl",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("ButterIsOnBowl") != true; },
                                [](IState::Ptr  state) -> void { wait(state->put("ButterIsOnBowl", true)->mulCost(state->atRef("FlourIsOnBowl") != true ? 0.9 : 1.0)); }),
        Goap::newPlanningAction("AddFlourToBowl",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("FlourIsOnBowl") != true; },
                                [](IState::Ptr  state) -> void { wait(state->put("FlourIsOnBowl", true)); }),
        Goap::newPlanningAction("MixBowlIngredients",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("IngredientsAreMixed") != true && state->atRef("EggIsOnBowl") == true && state->atRef("ButterIsOnBowl") == true && state->atRef("FlourIsOnBowl") == true; },
                                [](IState::Ptr  state) -> void { wait(state->put("IngredientsAreMixed", true)); }),
        Goap::newPlanningAction("PutBowlInOwen",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") != "Owen" && state->atRef("IngredientsAreMixed") == true && state->atRef("OwenTemperature") >= (REF_TEMP+100); },
                                [](IState::Ptr  state) -> void { wait(state->put("BowlLocation", "Owen")); }),
        Goap::newPlanningAction("BakePie",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Owen" && state->atRef("PieIsBaked") != true && state->atRef("BowlTemperature") >= (REF_TEMP+100); },
                                [](IState::Ptr  state) -> void { wait(state->put("PieIsBaked", true)); }),
        Goap::newPlanningAction("PieReadyForEat",
                                [](IState::CPtr state) -> bool { return state->atRef("PieIsBaked") == true && state->atRef("BowlLocation") == "Table" && state->atRef("BowlTemperature") < (REF_TEMP+30); },
                                [](IState::Ptr  state) -> void { wait(state->put("PieIsReadyForEat", true)); }),
        Goap::newPlanningAction("OrderPie",
                                [](IState::CPtr state) -> bool { return state->atRef("Credits") >= 8; },
                                [](IState::Ptr  state) -> void { wait(state->put("PieIsComing", 1)->add("Credits", -8)->mulCost(10)); }),
        Goap::newPlanningAction("ReceivePie",
                                [](IState::CPtr state) -> bool { return state->atRef("PieIsComing") >= 5; },
                                [](IState::Ptr  state) -> void { wait(state->put("PieIsReadyForEat", true)->put("PieIsComing", 0)); }),
        Goap::newPlanningAction("Wait",
                                [](IState::CPtr state) -> bool { (void)state; return true; },
                                [](IState::Ptr  state) -> void { wait(state->mulCost(0.9)); } )
    };
    IPlanner::Ptr planner = Goap::newPlanner(IPlanner::BreadthFirst, planningActions);
    return planner;
}

std::list<IPlanningAction::CPtr> backing_a_pie::MakePlan() {
    IPlanningStateComparer::Ptr numericalComparer = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON);
    if (!_planningStateMeter || !_planningStateMeter->goalState()->equals(_goalState)) {
        auto functionStateMeter = Goap::newFunctionStateMeter(_goalState);
        functionStateMeter->fnDistance([=](IState::CPtr state, IFunctionStateMeter::CPtr stateMeter) {
            float distanceToGoal = 1;
            distanceToGoal = stateMeter->numericStateMeter()->distance(state); // Distance to goal
            float distance = distanceToGoal;
            if (state->atRef("PieIsComing") == false) {
                // A conditional suggestion
                float distance1 = numericalComparer->distance(state, _backingHelper) * 0.8 + 0.2;
                //float distance2 = numericalComparer->distance(state, _restHelper) * 0.8 + 0.2;
                distance = distance1; // min(distance1, distance2);
            }
            if (state->atRef("OwenTemperature") == REF_TEMP ) {
                // A conditional suggestion
                distance = numericalComparer->distance(state, _orderHelper) * 0.8 + 0.2;
            }
            if (distanceToGoal > distance)
                distanceToGoal = distance;
            return distanceToGoal;
        });
        _planningStateMeter = functionStateMeter;
    }

    auto scopeTimer = Goap::newScopeTime("MakePlan: ", [](const char *szMessage, double time, const char *szUnits) {
        LOG(DEBUG) << szMessage << ": " << " actions, " << time << " " << szUnits;
    });
    std::list<IPlanningAction::CPtr> actionsArray;
    _planner->makePlanCached(_initialState, _planningStateMeter, actionsArray);
    LOG(DEBUG) << "Actions: " << actionsArray << ": " << " actions.";

    return actionsArray;
}
