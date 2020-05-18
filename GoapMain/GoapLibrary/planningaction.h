#ifndef GOAP_PLANNINGACTION_H
#define GOAP_PLANNINGACTION_H

#include "refcounter.h"
#include "newptr.h"
#include "goap/istate.h"
#include "goap/istatevalue.h"
#include "goap/iplanningaction.h"

namespace goap
{
/**
 * Action
 *
 * An action is a single, atomic step within a plan that makes a character do something.
 * Some possible actions include GotoPoint, ActivateObject, DrawWeapon,
 * ReloadWeapon, and Attack. The duration of an action may be short or infinitely
 * long. The ReloadWeaponaction will complete as soon as the character finishes a
 * reload animation. The Attackaction may continue infinitely, until the target is dead.
 *
 * Each action knows when it is valid to run, and what it will do to the game world. In other
 * words, an action knows its preconditionsand effects. Preconditions and effects provide a
 * mechanism for chaining actions into a valid sequence. For example, Attackhas a
 * precondition that the character’s weapon is loaded. The effect of ReloadWeaponis that
 * the weapon is loaded. It is east to see that ReloadWeaponfollowed by Attackis a
 * valid sequence of actions. Each action may have any number of preconditions and
 * effects.
 */
class PlanningAction : public virtual IPlanningAction
{
    IMPLEMENT_REFCOUNTER()

protected:
    IStringValue::CPtr _name;
    validator_function_type _validator; // State -> bool. Preconditions. The input State is not modified.
    executor_function_type _executor;   // State -> void. Effects. The input State is modified.

public:
    static PlanningAction::CPtr getNew(IStringValue::CPtr name_ = {}, validator_function_type validator_ = {}, executor_function_type executor_ = {})
    {
        PlanningAction::Ptr action = NewPtr<IPlanningAction>({}, name_, validator_, executor_);
        return action;
    }

    PlanningAction(IStringValue::CPtr name_ = {}, validator_function_type validator_ = {}, executor_function_type executor_ = {}) :
        _name(name_), _validator(validator_), _executor(executor_)
    {
    }

    void clear() {
        _name.reset();
        _validator = {};
        _executor = {};
    }

    executor_function_type executor() const
    {
        return _executor;
    }

    validator_function_type validator() const
    {
        return _validator;
    }

    IStringValue::CPtr name() const
    {
        return _name;
    }

    void setExecutor(executor_function_type executor_)
    {
        _executor = executor_;
    }

    void setValidator(validator_function_type validator_)
    {
        _validator = validator_;
    }

    void setName(IStringValue::CPtr name_)
    {
        _name = name_;
    }

    /**
     * Returns true if the action can be executed given this input state.
     */
    bool canExecute(IState::CPtr state) const override
    {
        return _validator(state);
    }

    /**
     * Returns a modified cloned state. The input state is not modified.
     */
    IState::Ptr execute(IState::CPtr state) const override
    {
        // The executor evolves input state to a new state, so we need to pass a copy to preserve the original
        IState::Ptr newState = dynamic_pointer_cast<IState>(state->clone());
        _executor(newState); // The input State is modified.
        //LOG(DEBUG) << "Execute\n from " << *state << "\n to " << *newState;
        return newState;
    }

    // IStringPrintable interface
public:
    string toDebugString() const override
    {
        return _name->toDebugString();
    }
    string toString() const override
    {
        return _name->toString();
    }
};

}

#endif // GOAP_PLANNINGACTION_H
