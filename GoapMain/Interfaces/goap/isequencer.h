#ifndef GOAP_ISEQUENCER_H
#define GOAP_ISEQUENCER_H

#include "common/irefcounter.h"
#include "istate.h"
#include "explicit_ptr.h"

namespace goap
{
    using namespace std;
    class IStateIterator;

    class ISequencer : public virtual IStringPrintable
    {
    public:
        typedef explicit_ptr<ISequencer> Ptr;
        typedef explicit_ptr<const ISequencer> CPtr;

        virtual float getInputTime() = 0;
        virtual void advanceInputTime(float ticks) = 0;
        virtual void pushState(const IState::CNew& state) = 0;
        virtual void pushString(const string& strState, float inc = 0) = 0;
        virtual IState::CPtr pullState() = 0;
        virtual IState::CPtr peekState() const = 0;
        virtual explicit_ptr<IStateIterator> iterator() = 0;
    };

}



#endif // GOAP_ISEQUENCER_H
