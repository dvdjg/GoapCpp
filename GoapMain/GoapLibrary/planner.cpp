#include "planner.h"
#include <sstream>
#include <unordered_set>
#include <functional>
#include <termcolor/termcolor.hpp>
#include "newptr.h"
#include "statesplan.h"
#include "log_hook.h"
#include "goaplibrary.h"

namespace goap
{


using namespace termcolor;

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
            //LOG(DEBUG) << "With action " << green << *action << reset << "\n Can execute state " << *reachedByPath;
            if (!action->canExecute(reachedByPath)) {
                break; // Do not continue
            }
            nextState = action->execute(reachedByPath); // Advance the state
            //LOG(DEBUG) << "With action " << green << *action << reset << "\n Can reach state " << *nextState;
            //reachedByPath->clear();
            reachedByPath = nextState;
            accCost += reachedByPath->cost();
        }
        reachedByPath->cost(accCost);
    } catch (std::exception e) {
        LOG(ERROR) << "[Planner] Error: Can't executeActions(). " << e.what();
    }
    LOG(DEBUG) << "Reached state: " << *reachedByPath;
    return reachedByPath;
}

std::list<IPlanningAction::CPtr> &Planner::makePlan(
        IState::CPtr initialState,
        IPlanningStateMeter::CPtr planningStateMeter,
        std::list<IPlanningAction::CPtr> &actionsArray,
        std::list<IState::CPtr> *pStates) {
    const bool isMonotonic = planningStateMeter->monotonic();
    float minDistance = 1;
    int64_t analyzedPaths = 0;
    int64_t rejectedPaths = 0;
    IPlanningAction::CPtr action;
    IState::CPtr stateReachedByPath;
    IPrioritized::Ptr unvisitedPaths_ = unvisitedPathes();

    std::unordered_set<IState::CPtr> visitedStates; // Should this be a set? std::unordered_map<IState::CPtr, IState::Ptr>
    visitedStates.insert(initialState);
    IPath::Ptr path; // In the first iteration, pathParent= null is the root

    auto gather = [&](float pathCost, float minDistance) -> float {
        IPath::Ptr plan;
        float min = minDistance;
        for (IPlanningAction::CPtr &action : _planningActions) {
            try {
                if (action->canExecute(stateReachedByPath)) {
                    IState::Ptr nextState = action->execute(stateReachedByPath);
                    float distance = planningStateMeter->distance(nextState);
                    //LOG(DEBUG) << "La accion \"" << green << *action << reset << "\" se puede ejecutar con una distancia=" << distance
                    //           << "\n    desde el estado: " << *stateReachedByPath
                    //           << "\n    hasta el estado: " << * nextState;
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
                    plan = Goap::newPath(action, path, pathCost + distance);
                    // Add the path to be analyzed after all the paths of this priority level be processed
                    unvisitedPaths_->pushLazy(plan);
                }
            } catch (exception e) {
                LOG(ERROR) << "[Planner] Error: " << e.what();
            }
        }
        if (!plan && path) {
            LOG(ERROR) << "[Planner] Discarded path: ";
            // The path has no children so it will not be used anymore
            path.reset();
        }
        return minDistance;
    };

    // Gather all the actions that can be reached from the intial state
    stateReachedByPath = initialState;
    minDistance = gather(0, minDistance); // First gathering

    if (unvisitedPaths_->empty()) {
        LOG(WARN) << "[MakePlan] There are no paths from the initial state!!!";
    } else {
        LOG(DEBUG) << "[MakePlan] There are " << unvisitedPaths_->size() << " unvisited paths\n " << *unvisitedPaths_ << " from the initial state:\n " << *stateReachedByPath;
    }
    bool bReachedGoal = false;
    // Search all the path forks neccesary to reach the goalState
    while (!unvisitedPaths_->empty()) {
        // Takes the path with the lower cost available
        path = unvisitedPaths_->pop();
        std::list<IPlanningAction::CPtr> actions;
        stateReachedByPath = path->executeFromRoot(initialState);
//        LOG(DEBUG) << "Executing actions " << green << path->getActions(actions) << reset << ", Total:" << actions.size() << " from the initial state "
//                   << "\n    to state " << *stateReachedByPath;
        // If this state was visited in other paths, do not compute it again
        auto itVisitedState = visitedStates.find(stateReachedByPath);
        if (itVisitedState != visitedStates.end()) {
            path.reset(); // The state this path advances to, was already visited
            ++rejectedPaths;
            continue;
        }
        // If this path takes us to the goal state, then return it
        bReachedGoal = planningStateMeter->enough(stateReachedByPath);
        if (bReachedGoal) {
            LOG(DEBUG) << red << "Reached Goal!! " << "Path cost=" << yellow << path->cost() << reset << "\n     Inverse path: " << *path; // This path has the costs modified by the hints of the functionStateMeter supplied
            path->getActions(actionsArray); // Get the planning actions from the intial state (reverse order)
            if (pStates) {
                path->getStates(initialState, *pStates);// Get the planning states from the intial state (reverse order)
            }
            path.reset();
            break;
        }
        // Mark the state as visited so we don't reanalyse it while searching other paths
        visitedStates.insert(stateReachedByPath);
        // Continue gathering all the actions that can be reached from current state "stateReachedByPath"
        actions.clear();
        //LOG(DEBUG) << "Added for action (" << yellow << "cost=" << path->cost() << reset << ") " << *path << " a new visited State. #VisitedStates=" << visitedStates.size()
        //           << "\n    to state.- " << *stateReachedByPath << "\n    Actions.- " << actionsArray;
        minDistance = gather(path->cost(), minDistance); // Gather paths to unvisitedPaths_
        ++analyzedPaths;
    }
    if (bReachedGoal) {
        LOG(INFO) << "[MakePlan] OK. analyzedPaths:" << analyzedPaths << ", rejectedPaths:" << rejectedPaths
                  << ", unvisitedPathes:"<< unvisitedPaths_->size();
    } else {
        LOG(WARN) << "[MakePlan] FAIL. analyzedPaths:" << analyzedPaths << ", rejectedPaths:" << rejectedPaths
                  << ", unvisitedPaths:" << unvisitedPaths_->size();
        actionsArray.clear();
    }
    return actionsArray;
}

/**
 * A version of makePlan() with the help of cached plans.
 * @note The found cached plan is not warranted to use the supplied planningStateMeter.
 */
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
            LOG(DEBUG) << "Target Goal State: " << *goalState;
            LOG(DEBUG) << "Real Goal State: " << *states.back();
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
    /**
     * Caches the list of states and their planning.
     * The 'states' vector must have one more element than 'plan'.
     * The last element of 'states' is its goal.
     * This function does not tries to reuse already cached 'states' and 'plan' so it
     * should be used after findPlanning() returned a zero length vector.
     */
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
    /**
     * Finds a plan in the cached data.
     * If the returning vector has a length of 0 elements, no plan has been found in cache.
     */
    std::list<IPlanningAction::CPtr> &retPlan = plan;
    size_t nStates;
    size_t nState;
    StatesPlan::Ptr nearStatePlan;
    float nearDistance = 1.0;
    int64_t nearOffset = -1;

    IPlanningStateComparer::Ptr planningStateComparer = NewPtr<IPlanningStateComparer>(NUMERICSTATECOMPARER_SINGLETON);
    // Search a cached plan for this goal
    std::list<StatesPlan::Ptr> &plannings = _statesCaches[goalState];
    if (!plannings.empty()) {
        //StatesPlan::lst_states_type::iterator itStates;
        for (auto &statePlan: plannings) {
            //auto itStatePlan = plannings.begin();
            //for (; itStatePlan != plannings.end(); ++itStatePlan) {
            //auto &statePlan = *itStatePlan;
            nStates = statePlan->states().size()-1;
            nState = 0;
            for (const IState::CPtr &storedState : statePlan->states()) {
                //for (itStates = statePlan->states().begin(); itStates != statePlan->states().end(); ++itStates) {
                //    const IState::CPtr &storedState = *itStates;
                float distance = planningStateComparer->distance(storedState, intialState);
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
                ++nState;
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
                auto statesPlan = nearStatePlan->plan();
                auto it1 = std::next(statesPlan.begin(), nState); // TODO: plan() --> vector
                retPlan.push_back(*it1);
            }
            // Test if the found plan is valid
            IState::Ptr reachedState = executeActions(retPlan, intialState);
            static const std::string strDiscr = STR_GOAP_NUMERICSTATECOMPARER_SINGLETON;
            bool bEnough = planningStateComparer->enough(reachedState, goalState);
            if (!bEnough) {
                // This plan is not good enough
                retPlan.clear();
            }
        }
    }
    return retPlan;
}

ostream & IPlanningAction::planToOstream(ostream &ss, const std::list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState) {
    float totalCost = 0;
    IPlanningAction::CPtr action;
    if (actionsArray.empty()) {
        ss << "No plan.";
    } else {
        IState::CPtr reachedByPath = initialState;
        if (reachedByPath) {
            totalCost += reachedByPath->cost();
            ss << "0. " << green << "init" << reset << " totalCost=" << totalCost << " state=" << *reachedByPath << "\n";
        }
        int64_t i = 0;
        for (auto &action : actionsArray) {
            if (!action) {
                break;
            }
            ss << (i+1) << ". " << green << *action << reset << " ";
            if (reachedByPath) {
                reachedByPath = action->execute(reachedByPath); // Advance the state
                totalCost += reachedByPath->cost();
                ss << "totalCost=" << totalCost << " state=" << *reachedByPath << "\n";
            }
            ++i;
        }
    }
    return ss;
}

string IPlanningAction::planToString(const std::list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState)
{
    std::stringstream ss;
    planToOstream(ss, actionsArray, initialState);
    return ss.str();
}

string Planner::toString() const
{
    return toDebugString();
}

string Planner::toDebugString() const
{
    std::stringstream ss;
    toOstream(ss);
    string str = ss.str();
    return str;
}

ostream &Planner::toOstream(ostream &ss) const
{
    ss << "Planner. planningMethod=" << _planningMethod << std::endl;
    int64_t i = 0;
    for (auto &action : _planningActions) {
        ss << " [" << i << "]. " << green << *action << reset << std::endl;
        ++i;
    }
    return ss << "]";
}

}


