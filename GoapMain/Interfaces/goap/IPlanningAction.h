#ifndef GOAP_IPLANNINGACTION_H
#define GOAP_IPLANNINGACTION_H

#include <list>
#include "common/irefcounter.h"
#include "istringvalue.h"
#include "istate.h"

namespace goap
{

class IPlanningAction : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IPlanningAction> Ptr;
    typedef explicit_ptr<const IPlanningAction> CPtr;

    virtual IStringValue::CPtr name() const = 0;

    /**
     * Returns true if the action can be executed given this input state.
     */
    virtual bool canExecute(IState::CPtr state) const = 0;

    /**
     * Returns a modified cloned state. The input state is not modified.
     */
    virtual IState::Ptr execute(IState::CPtr state) const = 0;

    static std::string planToString(const std::list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState = {});
};

}

#endif // GOAP_IPLANNINGACTION_H
