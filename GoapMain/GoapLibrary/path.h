#ifndef GOAP_PATH_H
#define GOAP_PATH_H

#include "refcounter.h"
#include "goap/ipath.h"
#include "goap/iplanningaction.h"

namespace goap
{
/**
 * Stores action information:
 * - Preconditions: What are the simplest state expressions?
 * - Modifications: What states changes and in what sense?
 * @todo
 */
class Path : public IPath
{
    IMPLEMENT_REFCOUNTER()

public:
    typedef explicit_ptr<Path> Ptr;
    typedef explicit_ptr<const Path> CPtr;

protected:
    IPlanningAction::CPtr _action;
    IPath::Ptr _parent;
    float _cost = 1;

    // Cache the map betwen a initial state and its final state
    IState::CPtr _initialState;
    IState::Ptr _finalState;

public:

    Path();
    Path(IPlanningAction::CPtr action_, IPath::Ptr parent_ = {}, float cost_ = 0);

    void clear();

    IPath::Ptr parent() const override;

    void parent(IPath::Ptr parent_);

    float cost() const override;

    void setCost(float cost_);

    float distance() const override;

    IPlanningAction::CPtr action() const override;

    void action(IPlanningAction::CPtr action_);

    IPath::Ptr addChild(IPlanningAction::Ptr node, float cost_) override;

    /**
     * Returns the final state of the secuence of actions this path represents.
     * This function is called very often when searching so we cache the result state.
     */
    IState::Ptr executeFromRoot(IState::CPtr initialState) override;

    /**
     * Returns the number of acctions of the path: from the root to this leave.
     */
    std::size_t getActionCount() override;

    /**
     * Get the actions from the root to this leave.
     * If the 'actions' parameter is given then actions will be unshift to the array.
     */
    void getActions(std::list<IPlanningAction::CPtr>& actions) const override;

    /**
     * Returns a list with the resulting states of the execution of the stored actions from the
     * supplied initialState.
     * This function caches the computed list of states so if it is used the same 'initialState'
     * of a prior call, it will be returned the cached results with no need of executing the actions again.
     */
    void getStates(IState::CPtr initialState, std::list<IState::CPtr>& states) override;
};


}

#endif // GOAP_PATH_H
