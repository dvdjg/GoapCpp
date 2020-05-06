#ifndef GOAP_ACTIONINFO_H
#define GOAP_ACTIONINFO_H

#include "refcounter.h"
#include "common/irefcounter.h"
#include "goap/iplanningaction.h"

namespace goap
{
/**
 * Stores action information:
 * - Preconditions: What are the simplest state expressions?
 * - Modifications: What states changes and in what sense?
 * @todo
 */
class ActionInfo : public virtual IRefCounter
{
    IMPLEMENT_REFCOUNTER()

protected:
    IPlanningAction::CPtr _action;

public:
    ActionInfo(IPlanningAction::CPtr action_) : _action(action_) {
    }

    IPlanningAction::CPtr action() {
        return _action;
    }
};

}

#endif // GOAP_ACTIONINFO_H
