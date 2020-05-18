#ifndef GOAP_IPATH_H
#define GOAP_IPATH_H

#include <list>
#include "goap/istringvalue.h"
#include "goap/istate.h"
#include "iplanningaction.h"

namespace goap
{
/**
 * The path is a component of the search tree.
 * The tree begins with a root Path element wich has no parent.
 * Any other Path has one parent and 0 or more children.
 * The Path holds an Action transforming its input State to an Output State.
 */
class IPath : public virtual IStringPrintable
{
public:
    typedef explicit_ptr<IPath> Ptr;
    typedef explicit_ptr<const IPath> CPtr;

    virtual IPath::Ptr parent() const = 0;

    virtual float cost() const = 0;

    virtual float distance() const = 0;

    virtual IPlanningAction::CPtr action() const = 0;

    virtual IPath::Ptr addChild(IPlanningAction::Ptr node, float cost_) = 0;

    /**
     * Returns the final state of the secuence of actions this path represents.
     * This function is called very often when searching so we cache the result state.
     */
    virtual IState::Ptr executeFromRoot(IState::CPtr initialState) = 0;

    /**
     * Returns the number of acctions of the path: from the root to this leave.
     */
    virtual std::size_t getActionCount() = 0;

    /**
     * Get the actions from the root to this leave.
     * If the 'actions' parameter is given then actions will be unshift to the array.
     */
    virtual std::list<IPlanningAction::CPtr>& getActions(std::list<IPlanningAction::CPtr>& actions_) const = 0;

    /**
     * Returns a list with the resulting states of the execution of the stored actions from the
     * supplied initialState.
     * This function caches the computed list of states so if it is used the same 'initialState'
     * of a prior call, it will be returned the cached results with no need of executing the actions again.
     */
    virtual std::list<IState::CPtr>& getStates(IState::CPtr initialState, std::list<IState::CPtr>& states) = 0;
};

}

#endif // GOAP_IPATH_H
