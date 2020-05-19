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
    typedef std::function<float(IState::CPtr)> validator_function_type;
    typedef std::function<void(IState::Ptr)> executor_function_type;

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

    static ostream & planToOstream(ostream &ss, const std::list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState = {});
    static std::string planToString(const std::list<IPlanningAction::CPtr> &actionsArray, IState::CPtr initialState = {});
};

inline static ostream& operator<<(ostream& os, const std::list<IPlanningAction::CPtr>& dt) {
    const char *sz = "[";
    for (auto &action : dt) {
        os << sz << *action;
        sz = ", ";
    }
    return os << "]";
}

}

#endif // GOAP_IPLANNINGACTION_H
