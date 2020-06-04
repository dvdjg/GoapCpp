#ifndef GOAP_ISEQUENCER_H
#define GOAP_ISEQUENCER_H

#include "common/irefcounter.h"
#include "ihashable.h"
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
        virtual IState::pair_state pullState() = 0;
        virtual IState::pair_state peekState() const = 0;
        virtual explicit_ptr<IStateIterator> iterator() = 0;
    };


    class IStateIterator : public IRefCounter {
    public:
        typedef explicit_ptr<IStateIterator> Ptr;
        typedef explicit_ptr<const IStateIterator> CPtr;

        virtual bool hasNext() const = 0;
        virtual IState::pair_state next() = 0;
        virtual IState::pair_state peekNext() = 0;
        virtual int64_t size() const = 0;
        virtual explicit_ptr<IStateIterator> clone() const = 0;
        //virtual IStateIterator* operator=(const IState::CPtr& container) = 0;
    };
}



#endif // GOAP_ISEQUENCER_H
