#ifndef GOAP_ISTATEITERATOR_H
#define GOAP_ISTATEITERATOR_H

#include "common/irefcounter.h"
#include "istate.h"
#include "explicit_ptr.h"

namespace goap
{
    using namespace std;

    class IStateIterator : public IRefCounter {
    public:
        typedef explicit_ptr<IStateIterator> Ptr;
        typedef explicit_ptr<const IStateIterator> CPtr;

        virtual bool hasNext() const = 0;
        virtual IState::CPtr next() = 0;
        virtual IState::CPtr peekNext() const = 0;
        virtual int64_t size() const = 0;
        virtual explicit_ptr<IStateIterator> clone() const = 0;
        //virtual IStateIterator* operator=(const IState::CPtr& container) = 0;
    };
}



#endif // GOAP_ISTATEITERATOR_H
