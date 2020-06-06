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

public:

    grammar_solver() {
    }

    grammar_solver(const grammar_solver& other) :
        _planner(other._planner),
        _initialState(other._initialState),
        _goalState(other._goalState),
        _plan(other._plan),
        _planningStateMeter(other._planningStateMeter) {
    }

    list<IPlanningAction::CPtr> makePlan(IState::New initial, IState::New goal) {
        grammar_plan(initial, goal);
        return makePlan();
    }

private:


    void grammar_plan(IState::New initial, IState::New goal) {
        _initialState = NewPtr<IState>()->assign(initial);
        _goalState = NewPtr<IState>()->assign(goal);
        _planner = planning_actions();
    }

    IState::Ptr wait(IState::Ptr state) {
        stackPeek(state);
        return state;
    }

    IPlanner::Ptr planning_actions() {
        list<IPlanningAction::CPtr> planningActions {
            Goap::newPlanningAction("Load",
                                    [=](IState::CPtr state) -> bool { return stackCanLoad(state);                               },
                                    [=](IState::Ptr  state) -> void { stackLoad(state); wait(state);                            }),
            Goap::newPlanningAction("Store",
                                    [=](IState::CPtr state) -> bool { return stackCanStore();                                   },
                                    [=](IState::Ptr  state) -> void { stackStore(state); wait(state);                           })
        };
        IPlanner::Ptr planner = Goap::newPlanner(IPlanner::BreadthFirst, planningActions);
        return planner;
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

        auto scopeTimer = Goap::newScopeTime("makePlan: ", [](const char *szMessage, double time, const char *szUnits) {
            LOG(DEBUG) << szMessage << " " << " actions, " << time << " " << szUnits;
        });
        list<IPlanningAction::CPtr> actionsArray;
        _planner->makePlanCached(_initialState, _planningStateMeter, actionsArray);
        LOG(DEBUG) << "Goal length=" << _goalState->size() << " deep, " << pow(2, _goalState->size() - 1) << " optimus, " << actionsArray.size() << " actions:\n   " << actionsArray;
        _plan = actionsArray;
        IPlanningAction::planToOstream(cerr, actionsArray, _initialState);

        return actionsArray;
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
