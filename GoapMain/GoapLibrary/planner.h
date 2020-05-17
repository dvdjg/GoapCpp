#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include <list>
#include <unordered_map>
#include "refcounter.h"
#include "statesplan.h"
#include "goap/iprioritized.h"
#include "goap/iplanner.h"
#include "goap/iplanningstatecomparer.h"

namespace goap
{

class Planner : public IPlanner
{
    IMPLEMENT_REFCOUNTER()

public:
    typedef explicit_ptr<Planner> Ptr;
    typedef explicit_ptr<const Planner> CPtr;

protected:
    type _planningMethod = type::BreadthFirst;

    /*std::list<IPlanningAction::CPtr> makePlan(
                IState::CPtr initialState,
                IPlanningStateMeter::CPtr planningStateMeter,
                std::list<IPlanningAction::CPtr> actionsArray = {},
                std::list<IState::CPtr> states */
    /**
     * The set of actions that can be taken.
     * Every action has a precondition to be met in order to be executed.
     * An action execution implies a state change.
     * These are the rules of the system.
     */
    std::list<IPlanningAction::CPtr> _planningActions;

public:
    Planner();

    Planner(IPlanner::type planningMethod_, const std::list<IPlanningAction::CPtr> &planningActions_);
    virtual void clear();

    const std::list<IPlanningAction::CPtr>& planningActions() const;
    void planningActions(const std::list<IPlanningAction::CPtr> &planningActions_);

    IPlanner::type planningMethod() override;
    void planningMethod(IPlanner::type method) override;

    /**
     * Helper function to compute the reached state resulting of executing all the allowed actions from
     * the initial state.
     * The cost of the resulting state is overwritten with the sum of all intermediate costs.
     */
    static IState::Ptr executeActions(const std::list<IPlanningAction::CPtr> &actions, IState::CPtr initialState);


    /**
     * Returns the plan: A Vector of IPlanningAction.
     * If no plan is found a null is returned.
     */
    std::list<IPlanningAction::CPtr> &makePlan (
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            std::list<IPlanningAction::CPtr> &actionsArray,
            std::list<IState::CPtr>* pStates = nullptr) override;

    /**
     * A version of makePlan() with the help of cached plans.
     * @note The found cached plan is not warranted to use the supplied planningStateMeter.
     */
    std::list<IPlanningAction::CPtr> &makePlanCached(
            IState::CPtr initialState,
            IPlanningStateMeter::CPtr planningStateMeter,
            std::list<IPlanningAction::CPtr> &actionsArray,
            std::list<IState::CPtr> *pStates) override;

private:
    IPrioritized::Ptr unvisitedPathes();

    // typedef std::unordered_map<IState::CPtr, IState::Ptr> states_dictionary_type;
private:
    std::unordered_map<IState::CPtr, std::list<StatesPlan::Ptr>> _statesCaches;

public:
    /**
     * Caches the list of states and their planning.
     * The 'states' vector must have one more element than 'plan'.
     * The last element of 'states' is its goal.
     * This function does not tries to reuse already cached 'states' and 'plan' so it
     * should be used after findPlanning() returned a zero length vector.
     */
    void cacheStates(const std::list<IState::CPtr> &states, const std::list<IPlanningAction::CPtr> &plan);

    /**
     * Finds a plan in the cached data.
     * If the returning vector has a length of 0 elements, no plan has been found in cache.
     */
    std::list<IPlanningAction::CPtr> &findPlan(const IState::CPtr &intialState,
            const IState::CPtr &goalState,
            std::list<IPlanningAction::CPtr> &plan);

    // IStringPrintable interface
public:
    std::string toDebugString() const override;
    std::string toString() const override;
};

}

#endif // GOAP_PLANNER_H
