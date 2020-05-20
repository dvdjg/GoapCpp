#ifndef GOAP_STATESPLAN_H
#define GOAP_STATESPLAN_H

#include <list>
//#include <vector>
#include "refcounter.h"
#include "goap/istate.h"
#include "goap/iplanningaction.h"

namespace goap
{

class StatesPlan : public IRefCounter {
    IMPLEMENT_REFCOUNTER()

    std::list<IState::CPtr> _states;
    std::list<IPlanningAction::CPtr> _plan;

public:
    typedef explicit_ptr<StatesPlan> Ptr;
    typedef explicit_ptr<const StatesPlan> CPtr;
    typedef std::list<IState::CPtr> lst_states_type;

    StatesPlan() {
    }

    std::list<IState::CPtr> states() const {
        return _states;
    }

    void states(const std::list<IState::CPtr> &value) {
        _states = value;
    }

    const std::list<IPlanningAction::CPtr> &plan() const {
        return _plan;
    }

    void plan(std::list<IPlanningAction::CPtr> value) {
        _plan = value;
    }
};

}

#endif // GOAP_STATESPLAN_H
