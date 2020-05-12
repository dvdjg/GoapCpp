#include "backing_a_pie.h"
#include "newptr.h"

float backing_a_pie::REF_TEMP = 300;

backing_a_pie::backing_a_pie()
{
    _backingHelper = NewPtr<IState>();
    _backingHelper->assign({{"EggIsOnBowl", 1.f}, {"ButterIsOnBowl", 1.f}, {"FlourIsOnBowl", 1.f}, {"IngredientsAreMixed", 1.f}, {"PieIsBaked", 1.f}});
    _orderHelper = NewPtr<IState>();
    _orderHelper->assign({{"PieIsComing", 5}});

    backing_actions();
}

void backing_a_pie::backing_plan(IState::map_string_float_type initial, IState::map_string_float_type goal)
{
    _initialState = NewPtr<IState> ({}, initial);
    _goalState = NewPtr<IState> ({}, goal);
}

void backing_a_pie::run()
{
    //std::list<IPlanningAction::Ptr> plan; // IEnumerable<IPlanningAction<State>>
    backing_plan({ {"OwenTemperature", REF_TEMP}, {"BowlTemperature", REF_TEMP}, {"Credits", 10} }, {{"PieIsReadyForEat", 1}});
    std::list<IPlanningAction::CPtr> plan = MakePlan();

    LOG(INFO) << "Plan:\n" << IPlanningAction::planToString(plan, _initialState);
}

IState::Ptr backing_a_pie::wait(IState::Ptr state)
{
    float OwenTemperature = state->atRef("OwenTemperature");
    if (state->atRef("OwenIsOn") == 1) {
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
    }else if (BowlTemperature > REF_TEMP) {
        BowlTemperature -= 20;
    }

    state->put("BowlTemperature", BowlTemperature);
    state->put("OwenTemperature", OwenTemperature);

    if (state->atRef("PieIsComing") > 0) {
        state->add("PieIsComing", 1);
    }
    return state;
}

void backing_a_pie::backing_actions()
{
    std::list<IPlanningAction::Ptr> planningActions {
        NewPtr<IPlanningAction>({},
                                "TurnOnOwen",
                                [](IState::CPtr state) -> bool { return state->atRef("OwenIsOn") != 1; },
        [](IState::Ptr  state) -> void { wait(state->put("OwenIsOn", 1));  }),
        NewPtr<IPlanningAction>({},
                                "TurnOffOwen",
                                [](IState::CPtr state) -> bool { return state->atRef("OwenIsOn") == true; },
        [](IState::Ptr  state) -> void { wait(state->put("OwenIsOn", false)); }),
        NewPtr<IPlanningAction>({},
                                "PutBowlOnTable",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") != "Table"; },
        [](IState::Ptr  state) -> void { wait(state->put("BowlLocation", "Table")); }),
        NewPtr<IPlanningAction>({},
                                "AddEggToBowl",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("EggIsOnBowl") != true; },
        [](IState::Ptr  state) -> void { wait(state->put("EggIsOnBowl", true)->mulCost(state->atRef("ButterIsOnBowl") != true ? 0.9 : 1.0)); }),
        NewPtr<IPlanningAction>({},
                                "AddButterToBowl",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("ButterIsOnBowl") != true; },
        [](IState::Ptr  state) -> void { wait(state->put("ButterIsOnBowl", true)->mulCost(state->atRef("FlourIsOnBowl") != true ? 0.9 : 1.0)); }),
        NewPtr<IPlanningAction>({},
                                "AddFlourToBowl",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && state->atRef("FlourIsOnBowl") != true; },
        [](IState::Ptr  state) -> void { wait(state->put("FlourIsOnBowl", true)); }),
        NewPtr<IPlanningAction>({},
                                "MixBowlIngredients",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Table" && /*state->atRef("IngredientsAreMixed") != true &&*/ state->atRef("EggIsOnBowl") == true && state->atRef("ButterIsOnBowl") == true && state->atRef("FlourIsOnBowl") == true; },
        [](IState::Ptr  state) -> void { wait(state->put("IngredientsAreMixed", true)); }),
        NewPtr<IPlanningAction>({},
                                "PutBowlInOwen",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") != "Owen" && state->atRef("IngredientsAreMixed") == true && state->atRef("OwenTemperature") >= (REF_TEMP+100); },
        [](IState::Ptr  state) -> void { wait(state->put("BowlLocation", "Owen")); }),
        NewPtr<IPlanningAction>({},
                                "BakePie",
                                [](IState::CPtr state) -> bool { return state->atRef("BowlLocation") == "Owen" && /*state->atRef("PieIsBaked") != true &&*/ state->atRef("BowlTemperature") >= (REF_TEMP+100); },
        [](IState::Ptr  state) -> void { wait(state->put("PieIsBaked", true)); }),
        NewPtr<IPlanningAction>({},
                                "PieReadyForEat",
                                [](IState::CPtr state) -> bool { return state->atRef("PieIsBaked") == true && state->atRef("BowlLocation") == "Table" && state->atRef("BowlTemperature") < (REF_TEMP+30); },
        [](IState::Ptr  state) -> void { wait(state->put("PieIsReadyForEat", true)); }),
        NewPtr<IPlanningAction>({},
                                "OrderPie",
                                [](IState::CPtr state) -> bool { return state->atRef("Credits") >= 8; },
        [](IState::Ptr  state) -> void { wait(state->put("PieIsComing", 1)->add("Credits", -8)->mulCost(13)); }),
        NewPtr<IPlanningAction>({},
                                "ReceivePie",
                                [](IState::CPtr state) -> bool { return state->atRef("PieIsComing") >= 5; },
        [](IState::Ptr  state) -> void { wait(state->put("PieIsReadyForEat", true)->put("PieIsComing", 0)); }),
        NewPtr<IPlanningAction>({},
                                "Wait",
                                [](IState::CPtr state) -> bool { (void)state; return true; },
        [](IState::Ptr  state) -> void { wait(state->mulCost(0.9)); } )
    };
    _planner = NewPtr<IPlanner>({}, IPlanner::BreadthFirst, planningActions);
}

std::list<IPlanningAction::CPtr> backing_a_pie::MakePlan()
{
    if (!_planningStateMeter || !_planningStateMeter->goalState()->equals(_goalState)) {
        auto functionStateMeter = NewPtr<FunctionStateMeter>({}, _goalState);
        functionStateMeter->fnDistance([=](IState::CPtr state, FunctionStateMeter::CPtr stateMeter)
        {
            float ret = 1;
            ret = stateMeter->numericStateMeter()->distance(state); // Distance to goal
            float distance = ret;
            if (state->atRef("PieIsComing") == 0) {
                // A conditional suggestion
                distance = NewPtr<IPlanningStateComparer>({STR_GOAP_NUMERICSTATECOMPARER_SINGLETON})->distance(state, _backingHelper) * 0.8 + 0.2;
                //distance = NumericStateComparer::singleton.distance(state, _backingHelper) * 0.8 + 0.2;
            }
            if (state->atRef("OwenTemperature") == REF_TEMP ) {
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
    auto fn = [](const char *szMessage, double time, const char *szUnits) {
        LOG(DEBUG) << szMessage << ": " << " actions, " << time << " " << szUnits;
    };
    auto scopeTimer = NewPtr<IScopeTime>({}, "MakePlan: ", (IScopeTime::pfn_time)fn);
    actionsArray = _planner->makePlanCached(_initialState, _planningStateMeter, actionsArray);
    LOG(DEBUG) << actionsArray.size() << ": " << " actions.";

    return actionsArray;
}
