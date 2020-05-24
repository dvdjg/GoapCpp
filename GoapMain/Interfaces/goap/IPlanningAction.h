#ifndef GOAP_IPLANNINGACTION_H
#define GOAP_IPLANNINGACTION_H

#include <list>
#include <functional>
#include <ostream>
#include "goap/istringvalue.h"
#include "istringvalue.h"
#include "istate.h"

namespace goap
{
using namespace std;

class IPlanningAction : public virtual IStringPrintable
{
public:
    typedef function<float(IState::CPtr)> validator_function_type;
    typedef function<void(IState::Ptr)> executor_function_type;

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

    static ostream & planToOstream(ostream &ss, const list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState = {});
    static string planToString(const list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState = {});
};

class planToOStream {
    const list<IPlanningAction::CPtr>& actionsArray;
    IState::CPtr initialState;
public:
    planToOStream(const list<IPlanningAction::CPtr>& actionsArray, IState::CPtr initialState = {}) : actionsArray(actionsArray), initialState(initialState) {
    }
    ostream& toOstream(ostream& ss) const {
        return IPlanningAction::planToOstream(ss, actionsArray, initialState);
    }
};

inline static ostream& operator<<(ostream& os, const planToOStream& pl) {
    return pl.toOstream(os);
}

inline static ostream& operator<<(ostream& os, const list<IPlanningAction::CPtr>& dt) {
    const char *sz = "[";
    for (auto &action : dt) {
        os << sz << *action;
        sz = ", ";
    }
    return os << "]";
}

}

#endif // GOAP_IPLANNINGACTION_H
