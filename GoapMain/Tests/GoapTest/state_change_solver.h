#ifndef STATE_CHANGE_SOLVER_H
#define STATE_CHANGE_SOLVER_H

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

class state_change_solver
{
    IPlanner::Ptr _planner;
    IState::CPtr _initialState;
    IState::CPtr _goalState;
    list<IPlanningAction::CPtr> _plan;
    list<IStateValue::CPtr> _inputSecuence; // Secuence of keys from the input state
    list<IStateValue::CPtr> _outputSecuence; // Secuence of keys to the output state
    list<IStateValue::Ptr> _stack;

    IPlanningStateMeter::CPtr _planningStateMeter;

public:
    state_change_solver() {
    }
    state_change_solver(const state_change_solver& other) :
        _planner(other._planner),
        _initialState(other._initialState),
        _goalState(other._goalState),
        _plan(other._plan),
        _planningStateMeter(other._planningStateMeter) {
    }

    list<IPlanningAction::CPtr> makePlan(IState::New initial, IState::New goal, int n = 3) {
        tower_plan(initial, goal, n);
        return makePlan();
    }

    void setInputSequence(initializer_list<IStateValue::CNew> inputSecuence) {
        _inputSecuence.assign(inputSecuence.begin(), inputSecuence.end());
    }

    void setInputSequence(list<IStateValue::CPtr> inputSecuence) {
        _inputSecuence = inputSecuence;
    }

    void setOutputSequence(initializer_list<IStateValue::CNew> outputSecuence) {
        _outputSecuence.assign(outputSecuence.begin(), outputSecuence.end());
    }

    void setOutputSequence(list<IStateValue::CPtr> outputSecuence) {
        _outputSecuence = outputSecuence;
    }

private:

    bool stackCanLoad(const IState::CPtr& state) {
        bool ret = false;
        if (!_inputSecuence.empty()) {
            IStateValue::CPtr key = _inputSecuence.front();
            IStateValue::Ptr value = state->at(key);
            ret = !value->empty();
        }
        return ret;
    }

    void stackLoad(const IState::Ptr& state) {
        if (_inputSecuence.empty()) {
            LOG(ERROR) << "Stack 'inputSecuence' is empty.";
        } else {
            IStateValue::CPtr key = _inputSecuence.front();
            _inputSecuence.pop_front();
            IStateValue::Ptr value = state->at(key);
            stackPush(value);
        }
    }

    bool stackCanStore(const IState::CPtr& state) {
        bool ret = !_outputSecuence.empty() && !_stack.empty();
        return ret;
    }

    void stackStore(const IState::Ptr& state) {
        if (_outputSecuence.empty()) {
            LOG(ERROR) << "Stack 'outputSecuence' is empty.";
        } else {
            IStateValue::CPtr key = _outputSecuence.front();
            _outputSecuence.pop_front();
            IStateValue::Ptr value = stackPop();
            state->put(key, value);
        }
    }

    bool stackCanOp1() {
        return !_stack.empty();
    }

    bool stackCanOp2() {
        return _stack.size() >= 2;
    }

    void stackPush(const IStateValue::New& value) {
        _stack.push_back(value);
    }

    IStateValue::Ptr stackPop() {
        IStateValue::Ptr value = _stack.back();
        _stack.pop_back();
        return value;
    }

    void tower_plan(IState::New initial, IState::New goal, int n = 3) {
        _initialState = NewPtr<IState>()->assign(initial);
        _goalState = NewPtr<IState>()->assign(goal);
        _planner = planning_actions(n);
    }

    IPlanner::Ptr planning_actions(int n = 3) {
        list<IPlanningAction::CPtr> planningActions {
                    Goap::newPlanningAction("Load",
                                            [=](IState::CPtr state) -> bool { return stackCanLoad(state);               },
                                            [=](IState::Ptr  state) -> void { stackLoad(state);                         }),
                    Goap::newPlanningAction("Store",
                                            [=](IState::CPtr state) -> bool { return stackCanStore(state);              },
                                            [=](IState::Ptr  state) -> void { stackStore(state);                        }),
                    Goap::newPlanningAction("Nop",
                                            [=](IState::CPtr state) -> bool { return true;                              },
                                            [=](IState::Ptr  state) -> void {                                           }),
                    Goap::newPlanningAction("Unset",
                                            [=](IState::CPtr state) -> bool { return stackCanOp1();                     },
                                            [=](IState::Ptr  state) -> void { _stack.pop_back();                        }),
                    Goap::newPlanningAction("Add",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() + *stackPop());     }),
                    Goap::newPlanningAction("Sub",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() - *stackPop());     }),
                    Goap::newPlanningAction("Mul",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() * *stackPop());     }),
                    Goap::newPlanningAction("Eq",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() == *stackPop());     }),
                    Goap::newPlanningAction("Neq",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() != *stackPop());    }),
                    Goap::newPlanningAction("Lt",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() < *stackPop());     }),
                    Goap::newPlanningAction("Gt",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() > *stackPop());     }),
                    Goap::newPlanningAction("Lte",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() <= *stackPop());    }),
                    Goap::newPlanningAction("Gte",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() >= *stackPop());    }),
                    Goap::newPlanningAction("Neg",
                                            [=](IState::CPtr state) -> bool { return stackCanOp1();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(-*stackPop());                  }),
                    Goap::newPlanningAction("Not",
                                            [=](IState::CPtr state) -> bool { return stackCanOp1();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(!*stackPop());                  }),
                    Goap::newPlanningAction("And",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() && *stackPop());    }),
                    Goap::newPlanningAction("Or",
                                            [=](IState::CPtr state) -> bool { return stackCanOp2();                     },
                                            [=](IState::Ptr  state) -> void { stackPush(*stackPop() || *stackPop());    })
//                    Goap::newPlanningAction("Inc",
//                                            [=](IState::CPtr state) -> bool { return stackCanOp1();                     },
//                                            [=](IState::Ptr  state) -> void { stackPush(++*stackPop());                  }),
//                    Goap::newPlanningAction("Dec",
//                                            [=](IState::CPtr state) -> bool { return stackCanOp1();                     },
//                                            [=](IState::Ptr  state) -> void { stackPush(--*stackPop());                  })
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

};

}

#endif // STATE_CHANGE_SOLVER_H
