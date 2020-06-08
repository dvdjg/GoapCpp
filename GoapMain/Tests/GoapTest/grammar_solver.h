#ifndef GRAMMAR_SOLVER_H
#define GRAMMAR_SOLVER_H

#include <list>
#include <map>
#include "goap/iplanner.h"
#include "goap/istate.h"
#include "goap/iscopetimer.h"
#include "goap/ifunctionstatemeter.h"
#include "goap/iplanningstatecomparer.h"
#include "newptr.h"
#include "log_hook.h"
#include "goaplibrary.h"

/*

*/
namespace goap
{
using namespace std;

class grammar_solver
{
    IPlanner::Ptr _planner;
    IState::CPtr _initialState;
    IState::CPtr _goalState;
    list<IPlanningAction::CPtr> _plan;

    IPlanningStateMeter::CPtr _planningStateMeter;

    map<string, IState::CPtr> _actionTypes; // State names are the actions
public:

    grammar_solver() {
        fillActionStates();
    }

    grammar_solver(const grammar_solver& other) :
        _planner(other._planner),
        _initialState(other._initialState),
        _goalState(other._goalState),
        _plan(other._plan),
        _planningStateMeter(other._planningStateMeter) {
        fillActionStates();
    }

    void putActionState(const string& name, const IState::CNew& state) {
        _actionTypes[name] = state;
    }
    const IState::CNew& getActionState(const string& name) {
        return _actionTypes[name];
    }
    void fillActionStates() {
        putActionState("onenine", { { "1", 1 }, {"2", 1 }, {"3", 1 }, {"4", 1 }, {"5", 1 }, {"6", 1 }, {"7", 1 }, {"8", 1 }, {"9", 1 } });
        putActionState("digit.1", { { "0", 1 } });
        putActionState("digit.2", { {"onenine", 1 } });
        putActionState("digits.1", { { "digit", 1 } });
        putActionState("digits.2", { { "digit", 1 }, {"digits", 2 } });
        putActionState("integer.1", { { "digit", 1 } });
        putActionState("integer.2", { {"onenine", 1 } , {"digits", 2 } });
        putActionState("integer.3", { { "-", 1 }, { "digit", 2 } });
        putActionState("integer.4", { { "-", 1 }, {"onenine", 2 } , { "digits", 3 } });
    }

    list<IPlanningAction::CPtr> makePlan(const IState::New& initial, const IState::New& goal) {
        grammar_plan(initial, goal);
        return makePlan();
    }

    void grammar_plan(const IState::New& initial, const IState::New& goal) {
        _initialState = NewPtr<IState>()->assign(initial);
        _goalState = NewPtr<IState>()->assign(goal);
        _planner = planning_actions();
    }

    list<IPlanningAction::CPtr> makePlan() {
        IPlanningStateComparer::Ptr comparer = NewPtr<IPlanningStateComparer>(EXACTSTATEMETER_SINGLETON);
        if (!_planningStateMeter || *_planningStateMeter->goalState() != *_goalState) {
            auto functionStateMeter = Goap::newFunctionStateMeter(_goalState);
            functionStateMeter->monotonic(true);
            functionStateMeter->fnDistance([=](IState::CPtr state, IFunctionStateMeter::CPtr stateMeter) {
                float distanceToGoal = 1;
                distanceToGoal = stateMeter->exactStateMeter()->distance(state); // Distance to goal
                return distanceToGoal;
                });
            functionStateMeter->fnEnough([](IState::CPtr state, IFunctionStateMeter::CPtr stateMeter) -> bool {
                return stateMeter->exactStateMeter()->enough(state);
                });
            _planningStateMeter = functionStateMeter;
        }

        auto scopeTimer = Goap::newScopeTime("makePlan: ", [](const char* szMessage, double time, const char* szUnits) {
            LOG(DEBUG) << szMessage << " " << " actions, " << time << " " << szUnits;
            });
        list<IPlanningAction::CPtr> actionsArray;
        _planner->makePlanCached(_initialState, _planningStateMeter, actionsArray);
        LOG(DEBUG) << "Goal length=" << _goalState->size() << " deep, " << pow(2, _goalState->size() - 1) << " optimus, " << actionsArray.size() << " actions:\n   " << actionsArray;
        _plan = actionsArray;
        IPlanningAction::planToOstream(cerr, actionsArray, _initialState);

        return actionsArray;
    }

private:


    IState::Ptr wait(IState::Ptr state) {
        return state;
    }

    IPlanner::Ptr planning_actions() {
        //const auto digits = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
        IPlanningStateComparer::Ptr numericalComparer = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON);
        list<IPlanningAction::CPtr> planningActions;
        //list<IPlanningAction::CPtr> planningActions{
        //    Goap::newPlanningAction("onenine", [=](IState::CPtr state) -> bool { 
        //            const IState::CNew& actionState = getActionState("onenine");
        //            float distance = numericalComparer->distance(actionState, state);
        //            return distance < 0.2f;
        //        }, [=](IState::Ptr state) -> void {
        //            const IState::CNew& actionState = getActionState("onenine");
        //            IState::Ptr stMul = mul(actionState, state);
        //            auto pair = stMul->at(0);
        //            state->assign({ { "onenine" , pair.first } });
        //            state->mergeSequence("strExpression", true);
        //        }),
        //    Goap::newPlanningAction("digit.1", [=](IState::CPtr state) -> bool {
        //            const IState::CNew& actionState = getActionState("digit.1");
        //            float distance = numericalComparer->distance(actionState, state);
        //            return distance < 0.2f;
        //        }, [=](IState::Ptr state) -> void {
        //            state->assign({ { "onenine" , 1 } });
        //            state->mergeSequence("strExpression", true);
        //        })
        //};
        for (auto pair : _actionTypes) {
            string actionName = pair.first;
            IPlanningAction::CPtr pa = Goap::newPlanningAction(actionName, [=](IState::CPtr state) -> bool {
                    const IState::CNew& actionState = getActionState(actionName);
                    float distance = numericalComparer->distance(actionState, state);
                    LOG(DEBUG) << "Verifying '" << actionName << "' distance: " << distance << " for state " << *state;
                    return distance < 0.2f;
                }, [=](IState::Ptr state) -> void {
                    const IState::CNew& actionState = getActionState(actionName);
                    IState::Ptr stMul = mul(actionState, state);
                    auto pair = stMul->at(0);
                    size_t pos = actionName.find('.');
                    string tr = actionName.substr(0, pos);
                    state->assign({ {tr , pair.first } });
                    state->mergeSequence("strExpression", true);
                    LOG(DEBUG) << "Merged '" << actionName << "' into: " << *state;
                });
            planningActions.push_back(pa);
        }
        IPlanner::Ptr planner = Goap::newPlanner(IPlanner::BreadthFirst, planningActions);
        return planner;
    }

public:
    void run()
    {
        list<IPlanningAction::CPtr> plan;
        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3} }, { {"C1", 1}, {"C2", 2}, {"C3", 3} });
    }

    list<IPlanningAction::CPtr> plan() const {
        return _plan;
    }

    IState::CPtr initialState() const {
        return _initialState;
    }

};

}

#endif // GRAMMAR_SOLVER_H
