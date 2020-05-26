#ifndef HANOI_TOWER_SOLVER_H
#define HANOI_TOWER_SOLVER_H

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
class hanoi_tower_solver
{
    IPlanner::Ptr _planner;
    IState::CPtr _initialState;
    IState::CPtr _goalState;
    int _n = 0;
    list<IPlanningAction::CPtr> _plan;

    IPlanningStateMeter::CPtr _planningStateMeter;

public:
    hanoi_tower_solver() {
    }
    hanoi_tower_solver(const hanoi_tower_solver& other) :
        _planner(other._planner),
        _initialState(other._initialState),
        _goalState(other._goalState),
        _n(other._n),
        _plan(other._plan),
        _planningStateMeter(other._planningStateMeter) {
    }

    list<IPlanningAction::CPtr> makePlan(const IState::New& initial, const IState::New& goal, int n = 3) {
        tower_plan(initial, goal, n);
        return makePlan();
    }

private:
    void tower_plan(const IState::New& initial, const IState::New& goal, int n = 3) {
        _n = n;
        _initialState = NewPtr<IState>()->assign(initial);
        _goalState = NewPtr<IState>()->assign(goal);
        _planner = planning_actions(n);
    }

    IPlanner::Ptr planning_actions(int n = 3) {
        list<IPlanningAction::CPtr> planningActions {
            Goap::newPlanningAction("Move from A to B",
                                    [=](IState::CPtr state) -> bool { return validate(state, "A", "B", n); },
                                    [=](IState::Ptr  state) -> void {            move(state, "A", "B", n);  }),
            Goap::newPlanningAction("Move from A to C",
                                    [=](IState::CPtr state) -> bool { return validate(state, "A", "C", n); },
                                    [=](IState::Ptr  state) -> void {            move(state, "A", "C", n); }),
            Goap::newPlanningAction("Move from B to A",
                                    [=](IState::CPtr state) -> bool { return validate(state, "B", "A", n); },
                                    [=](IState::Ptr  state) -> void {            move(state, "B", "A", n); }),
            Goap::newPlanningAction("Move from B to C",
                                    [=](IState::CPtr state) -> bool { return validate(state, "B", "C", n); },
                                    [=](IState::Ptr  state) -> void {            move(state, "B", "C", n); }),
            Goap::newPlanningAction("Move from C to A",
                                    [=](IState::CPtr state) -> bool { return validate(state, "C", "A", n); },
                                    [=](IState::Ptr  state) -> void {            move(state, "C", "A", n); }),
            Goap::newPlanningAction("Move from C to B",
                                    [=](IState::CPtr state) -> bool { return validate(state, "C", "B", n); },
                                    [=](IState::Ptr  state) -> void {            move(state, "C", "B", n); })
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
                int an = state->atRef(concatStringInt("A", _n));
                if (an == _n) {
                    // A conditional suggestion: First remove all elements from the tower 'A'
                    float distance = comparer->distance(state, _initialState);
                    //float distance2 = numericalComparer->distance(state, _restHelper) * 0.8 + 0.2;
                    distance = 1.0f - distance / (2 * _n + 1.0f);
                    if (distanceToGoal > distance)
                        distanceToGoal = distance;
                }
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


//    public function run2():void
//    {
//        var Plan:Vector.<IPlanningAction>;
//        tower_plan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, "C1":null, "C2":null, "C3":null }, { {"C1", 1}, {"C2", 2}, {"C3", 3} }, 3);
//        plan = makePlan();

//        tower_plan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7}, {"A8", 8} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7}, {"C8", 8} }, 8);
//        plan = makePlan();
//    }

    // A1 1  B1 |  C1 |
    // A2 2  B2 |  C2 |
    // A3 3  B3 |  C3 |
    //   / \   / \   / \

public:
    void run()
    {
        list<IPlanningAction::CPtr> plan;

        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3} }, { {"C1", 1}, {"C2", 2}, {"C3", 3} }, 3);

        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4} }, 4);

        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5} }, 5);

        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6} }, 6);

        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);

        plan = makePlan( { {"A1", 1}, {"A2", 2}, {"A3", 3}, {"A4", 4}, {"A5", 5}, {"A6", 6}, {"A7", 7}, {"A8", 8} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7}, {"C8", 8} }, 8);

        plan = makePlan( { {"A3", 1}, {"A4", 2}, {"A5", 3}, {"A6", 4}, {"A7", 5}, {"B6", 6}, {"B7", 7} }, { {"C1", 1}, {"C2", 2}, {"C3", 3}, {"C4", 4}, {"C5", 5}, {"C6", 6}, {"C7", 7} }, 7);
    }

    list<IPlanningAction::CPtr> plan() const {
        return _plan;
    }

    IState::CPtr initialState() const {
        return _initialState;
    }

    //private var _stringBuffer:StringBuffer = new StringBuffer;
    /**
     * Test if an action is posible given a state
     * Validates that the state can change in the way it is specified in the 'from' 'to' strnigs.
     * @param n The max tower height.
     */
    static bool validate(IState::CPtr state, const string &from, const string &to, int n = 3)
    {
        int i = 0;
        IStateValue::CPtr val;
        int onFromTop = 0;
        for (i = 1; i <= n; ++i) {
            val = state->at(concatStringInt(from, i));
            if (val && !val->empty()) {
                break;
            }
        }
        if (!val || !val->isInt())
            return false;

        onFromTop = static_cast<int>(*val);

        int onToTop = 0;
        bool trigerTo = false;
        val.reset();
        for (i = 1; i <= n; ++i) {
            val = state->at(concatStringInt(to, i));
            if (val && !val->empty())
                break;
        }
        if (val && val->isInt()) {
            trigerTo = true;
            onToTop = static_cast<int>(*val);
        }
        return !trigerTo || onToTop > onFromTop;
    }

    //private static var _strCache:Object = { };
    /**
     * Helper function to cache strings in the form: 'String' + 'int'
     * This is a way of lowering pression from the GC.
     * The integger must be a low number.
     */
    static const string& concatStringInt(const string &str, int n)
    {
        static map<int, map<string, string>> mapStr;
        auto &val = mapStr[n][str];
        if (val.empty()) {
            val = str + to_string(n);
        }
        return val;
    }

    // The 'executor' function
    static void move(IState::Ptr state, const string &from, const string &to, int n = 3)
    {
        int i = 0;
        IStateValue::CPtr val;
        const string* a = nullptr;
        for (i = 1; i <= n; ++i) {
            const string& astr = concatStringInt(from, i);
            val = state->at(astr);
            if (val && !val->empty()) {
                a = &astr;
                break;
            }
        }
        if (!a) {
            throw runtime_error("Can't trig from '" + from + "' to '" + to + "'");
        }
        const string* b = nullptr;
        bool trigerTo = false;
        for (i = 1; i <= n; ++i) {
            val = state->at(concatStringInt(to, i));
            if (val && !val->empty()) {
                if (i-- == 1) {
                    throw runtime_error("Can't put to '" + to + "'"); // Tower is full
                } else {
                    b = &concatStringInt(to, i);
                }
                trigerTo = true;
                break;
            }
        }
        if (!trigerTo) {
            b = &concatStringInt(to, n);
        }
        // Update the source state
        state->put(*b, state->at(*a));
        state->remove(*a);
    }
};

}

#endif // HANOI_TOWER_SOLVER_H
