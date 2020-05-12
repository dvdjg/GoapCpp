#ifndef GOAP_IFUNCTIONSTATEMETER_H
#define GOAP_IFUNCTIONSTATEMETER_H

#include <functional>
#include "explicit_ptr.h"
#include "iplanningstatemeter.h"
#include "istate.h"

#define STR_GOAP_COMPARERSTATEMETER "ComparerStateMeter"
#define STR_GOAP_FUNCTIONSTATEMETER "FunctionStateMeter"


namespace goap
{

class IFunctionStateMeter : public virtual IPlanningStateMeter
{
public:
    typedef explicit_ptr<IFunctionStateMeter> Ptr;
    typedef explicit_ptr<const IFunctionStateMeter> CPtr;

    typedef std::function<float(IState::CPtr, IFunctionStateMeter::CPtr)> distance_function_type;
    typedef std::function<bool(IState::CPtr, IFunctionStateMeter::CPtr)> enough_function_type;

    virtual void addStateMeter(const std::string& name, IPlanningStateMeter::Ptr stateMeter) = 0;

    virtual void addStateMeter(IStateValue::CPtr name, IPlanningStateMeter::Ptr stateMeter) = 0;

    virtual IPlanningStateMeter::CPtr getStateMeter(IStateValue::CPtr name) = 0;

    virtual IPlanningStateMeter::CPtr getStateMeter(const std::string& name) = 0;

    virtual distance_function_type fnDistance() const = 0;

    virtual void fnDistance(distance_function_type value) = 0;

    virtual enough_function_type fnEnough() const = 0;

    virtual void fnEnough(enough_function_type value) = 0;

    virtual IPlanningStateMeter::Ptr numericStateMeter() const = 0;

    virtual IPlanningStateMeter::Ptr exactStateMeter() const = 0;
};

}

#endif // GOAP_IFUNCTIONSTATEMETER_H
