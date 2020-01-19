#ifndef IPLANNINGACTION_H
#define IPLANNINGACTION_H

#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "iclonable.h"
#include "istringvalue.h"
#include "goap/istate.h"
#include "goap/istringvalue.h"

namespace goap
{

class IPlanningAction : public IClonable, public virtual IRefCounter
{
public:
    IStringValue::CPtr name() const = 0;
    /**
     * Returns true if the action can be executed given this input state.
     */
    bool canExecute(IState::CPtr state) const = 0;
    /**
     * Returns a modified cloned state. The input state is not modified.
     */
    IState::Ptr execute(state:IReadState) const = 0;
};


#endif // IPLANNINGACTION_H
