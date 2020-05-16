#include "planner.h"
#include <sstream>
#include <functional>
#include "newptr.h"
#include "statesplan.h"
#include "log_hook.h"
#include "goaplibrary.h"

namespace goap
{

Planner::Planner(){
}

Planner::Planner(Planner::type planningMethod_, const std::list<IPlanningAction::CPtr> &planningActions_) {
    _planningMethod = planningMethod_;
    _planningActions = planningActions_;
}

void Planner::clear() {
    _planningMethod = type::BreadthFirst;
    _planningActions.clear();

}

const std::list<IPlanningAction::CPtr> &Planner::planningActions() const {
    return _planningActions;
}

void Planner::planningActions(const std::list<IPlanningAction::CPtr> &planningActions_) {
    _planningActions = planningActions_;
}

IPlanner::type Planner::planningMethod() {
    return _planningMethod;
}

void Planner::planningMethod(IPlanner::type method)  {
    _planningMethod = method;
}

IState::Ptr Planner::executeActions(const std::list<IPlanningAction::CPtr> &actions, IState::CPtr initialState) {
    IState::Ptr nextState;
    IState::Ptr initial = dynamic_pointer_cast<IState>(initialState->clone());
    IState::Ptr reachedByPath = initial;
    reachedByPath->cost(initialState->cost());
    try {
        float accCost = reachedByPath->cost();
        for(auto& action : actions) {
            if (!action->canExecute(reachedByPath)) {
                break; // Do not continue
            }
            LOG(ERROR) << "Executing Action: " << *action;
            nextState = action->execute(reachedByPath); // Advance the state
            //reachedByPath->clear();
            reachedByPath = nextState;
            accCost += reachedByPath->cost();
        }
        reachedByPath->cost(accCost);
    } catch (std::exception e) {
        LOG(ERROR) << "[Planner] Error: Can't executeActions(). " << e.what();
    }
    return reachedByPath;
}

std::list<IPlanningAction::CPtr> &Planner::makePlan(
        IState::CPtr initialState,
        IPlanningStateMeter::CPtr planningStateMeter,
        std::list<IPlanningAction::CPtr> &actionsArray,
        std::list<IState::CPtr> *pStates) {
    const bool isMonotonic = planningStateMeter->monotonic();
    float minDistance = 1;
    int analyzedPaths = 0;
    int rejectedPaths = 0;
    //float pathCost = 0;
    IPlanningAction::CPtr action;
    IState::CPtr reachedByPath;
    IPrioritized::Ptr unvisitedPathes_ = this->unvisitedPathes();
    states_dictionary_type visitedStates; // std::unordered_map<IState::CPtr, IState::Ptr>
    visitedStates[initialState] = initialState;
    IPath::Ptr path;

    auto gather = [&](float pathCost, float minDistance) -> float {
        float min = minDistance;
        for (auto &action : _planningActions) {
            try {
                if (action->canExecute(reachedByPath)) {
                    LOG(DEBUG) << "La accion \"" << *action << "\" puede ejecutar: " << *reachedByPath;
                    IState::Ptr nextState = action->execute(reachedByPath);
                    float distance = planningStateMeter->distance(nextState);
                    // Adds a new path leave
                    if (isMonotonic && min > distance) {
                        // When monotonic Meters are used, a decreasing in the distance to the goal
                        // found in a path increases the preference to explore that path.
                        minDistance = distance;
                        distance *= nextState->cost();
                        // Use a very low cost to give more chances for this path to be analyzed
                        distance = 4*distance - pathCost - 10; // Landmark: Put the distance betwen -10 and -6
                    } else {
                        distance *= nextState->cost();
                    }
                    IPath::Ptr plan = Goap::newPath(action, path, pathCost + distance);
                    // Add the path to be analyzed after all the paths of this priority level be processed
                    unvisitedPathes_->pushLazy(plan);
                }
            } catch (exception e) {
                LOG(ERROR) << "[Planner] Error: " << e.what();
            }
        }
        return minDistance;
    };

    // Gather all the actions that can be reached from the intial state
    reachedByPath = initialState;
    minDistance = gather(0, minDistance);

    if (unvisitedPathes_->empty()) {
        LOG(WARN) << "[MakePlan] There are no paths from the initial state!!!";
    }
    bool bReachedGoal = false;
    // Search all the path forks neccesary to reach the goalState
    while (!unvisitedPathes_->empty()) {
        // Takes the path with the lower cost available
        path = unvisitedPathes_->pop();
        reachedByPath = path->executeFromRoot(initialState);
        // If this state was visited in other paths, do not compute it again
        auto itVisitedState = visitedStates.find(reachedByPath);
        if (itVisitedState != visitedStates.end()) {
            //path.dispose();
            path.reset();
            ++rejectedPaths;
            continue;
        }
        //IState::CPtr visitedState = itVisitedState->second;
        // If this path takes us to the goal state, then return it
        bReachedGoal = planningStateMeter->enough(reachedByPath);
        if (bReachedGoal) {
            path->getActions(actionsArray); // Get the planning actions from the intial state
            if (pStates) {
                path->getStates(initialState, *pStates);
            }
            path.reset();
            break;
        }
        // Mark the state as visited so we don't reanalyse it while searching other paths
        visitedStates[reachedByPath] = reachedByPath;
        // Continue gathering all the actions that can be reached from current state
        minDistance = gather(path->cost(), minDistance);
        ++analyzedPaths;
    }
    if (bReachedGoal) {
        LOG(INFO) << "[MakePlan] OK. analyzedPaths:" << analyzedPaths << ", rejectedPaths:" << rejectedPaths
                  << ", unvisitedPathes:"<< unvisitedPathes_->size();
    } else {
        LOG(WARN) << "[MakePlan] FAIL. analyzedPaths:" << analyzedPaths << ", rejectedPaths:" << rejectedPaths
                  << ", unvisitedPaths:" << unvisitedPathes_->size();
        actionsArray.clear();
    }
    return actionsArray;
}

std::list<IPlanningAction::CPtr> &Planner::makePlanCached(
        IState::CPtr initialState,
        IPlanningStateMeter::CPtr planningStateMeter,
        std::list<IPlanningAction::CPtr> &actionsArray,
        std::list<IState::CPtr> *pStates)
{
    IState::Ptr goalState = dynamic_pointer_cast<IState>(planningStateMeter->goalState()->clone());
    actionsArray = findPlan(initialState, goalState, actionsArray);
    if (actionsArray.empty()) {
        std::list<IState::CPtr> states;
        if (!pStates) {
            pStates = &states;
        }
        actionsArray = makePlan(initialState, planningStateMeter, actionsArray, pStates);
        if (!actionsArray.empty()) {
            // Substitute the reached state for a clean goalState
            float cost = states.back()->cost();
            goalState->cost(cost);
            states.back() = goalState;
            states.push_front(initialState); // Add the initial state to the list
            cacheStates(states, actionsArray);
        }
    }
    return actionsArray;
}

IPrioritized::Ptr Planner::unvisitedPathes() {
    IPrioritized::Ptr prioritized;
    static const std::string strDiscrQueue = STR_GOAP_PRIORITIZED_QUEUE;
    static const std::string strDiscrStack = STR_GOAP_PRIORITIZED_STACK;
    switch (_planningMethod)
    {
    case type::BreadthFirst:
        prioritized = NewPtr<IPrioritized>(strDiscrQueue);
        break;
    case type::DepthFirst:
        prioritized = NewPtr<IPrioritized>(strDiscrStack);
        break;
    }
    return prioritized;
}

void Planner::cacheStates(const std::list<IState::CPtr> &states, const std::list<IPlanningAction::CPtr> &plan)
{
    IState::CPtr goalState = states.back();
    std::list<StatesPlan::Ptr> &plannings = _statesCaches[goalState];
    StatesPlan::Ptr statePlan = NewPtr<StatesPlan>();
    statePlan->states(states);
    statePlan->plan(plan);
    plannings.push_back(statePlan);
}

std::list<IPlanningAction::CPtr>& Planner::findPlan(
        const IState::CPtr &intialState,
        const IState::CPtr &goalState,
        std::list<IPlanningAction::CPtr> &plan) {
    std::list<IPlanningAction::CPtr> &retPlan = plan;
    size_t nStates;
    size_t nState;
    StatesPlan::Ptr nearStatePlan;
    float nearDistance = 1.0;
    int nearOffset = -1;

    // Search a cached plan for this goal
    std::list<StatesPlan::Ptr> &plannings = _statesCaches[goalState];
    if (!plannings.empty()) {
        StatesPlan::lst_states_type::iterator itStates;
        for (auto &statePlan: plannings) {
            //auto itStatePlan = plannings.begin();
            //for (; itStatePlan != plannings.end(); ++itStatePlan) {
            //auto &statePlan = *itStatePlan;
            nStates = statePlan->states().size()-1;
            nState = 0;
            for (const IState::CPtr &storedState : statePlan->states()) {
                //for (itStates = statePlan->states().begin(); itStates != statePlan->states().end(); ++itStates) {
                //    const IState::CPtr &storedState = *itStates;
                float distance = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON)->distance(storedState, intialState);
                if (nearDistance > distance) {
                    nearDistance = distance;
                    nearStatePlan = statePlan;
                    nearOffset = nState;
                    if (nearDistance <= 0.0) {
                        break;
                    }
                }
                if (nState < nStates) {
                    break;
                }
                nState++;
            }
            if (nearDistance <= 0.0) {
                break;
            }
        }
        if (nearOffset >= 0) {
            nStates = nearStatePlan->plan().size();
            //retPlan.length = nStates - nearOffset;
            // Copy the necessary portion of actions array
            //i = 0;
            for (nState = nearOffset; nState < nStates; nState++ ) {
                auto it1 = std::next(nearStatePlan->plan().begin(), nState); // TODO: plan() --> vector
                retPlan.push_back(*it1);
            }
            // Test if the found plan is valid
            IState::Ptr reachedState = executeActions(retPlan, intialState);
            static const std::string strDiscr = STR_GOAP_NUMERICSTATECOMPARER_SINGLETON;
            bool bEnough = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON)->enough(reachedState, goalState);
            if (!bEnough) {
                // This plan is not good enough
                retPlan.clear();
            }
        }
    }
    return retPlan;
}

//std::list<IPlanningAction::CPtr> Planner::makePlan(IState::CPtr initialState, IPlanningStateMeter::CPtr planningStateMeter, std::list<IPlanningAction::CPtr> actionsArray, std::list<IState::CPtr> states)
//{

//}

string IPlanningAction::planToString(const std::list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState)
{
    float totalCost = 0;
    std::stringstream ret;
    IPlanningAction::CPtr action;
    if (actionsArray.empty()) {
        ret << "No plan.";
    } else {
        IState::CPtr reachedByPath = initialState;
        if (reachedByPath) {
            totalCost += reachedByPath->cost();
            ret << "0.\"init\" totalCost=" << totalCost << " state=" << reachedByPath << "\n";
        }
        int i = 0;
        for (auto &action : actionsArray) {
            if (!action) {
                break;
            }
            ret << (i+1) << ".\"" << action << "\" ";
            if (reachedByPath) {
                reachedByPath = action->execute(reachedByPath); // Advance the state
                totalCost += reachedByPath->cost();
                ret << "totalCost=" << totalCost << " state=" << reachedByPath << "\n";
            }
            ++i;
        }
    }
    return ret.str();
}

std::string Planner::toDebugString() const {
    std::stringstream ret;
    ret << "Planner. planningMethod=" << _planningMethod << std::endl;
    int i = 0;
    for (auto &action : _planningActions) {
        ret << " [" << i << "]. " << *action << std::endl;
        ++i;
    }
    return ret.str();
}

std::string Planner::toString() const {
    return toDebugString();
}

}


