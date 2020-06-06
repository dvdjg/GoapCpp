#ifndef GOAP_SEQUENCER_H
#define GOAP_SEQUENCER_H

#include <list>
#include "goap/isequencer.h"
#include "goap/istateiterator.h"

#include "refcounter.h"

namespace goap
{
    using namespace std;
    class StateIterator;
    class Sequencer : public virtual ISequencer
    {
        IMPLEMENT_REFCOUNTER()
        friend StateIterator;
    public:
        typedef list<IState::CPtr> sequencer_list_type;

    private:
        float _time = 0;
        sequencer_list_type _lstStates;

    public:
        typedef explicit_ptr<Sequencer> Ptr;
        typedef explicit_ptr<const Sequencer> CPtr;
        void clear();

        float getInputTime() override;
        void advanceInputTime(float ticks) override;
        void pushState(const IState::CNew& state) override;
        IState::CPtr pullState() override;
        IState::CPtr peekState() const override;
        explicit_ptr<IStateIterator> iterator() override;

        // Heredado vía ISequencer
        virtual std::string toDebugString() const override;
        virtual std::string toString() const override;
        virtual ostream& toOstream(ostream& os) const override;
        virtual void pushString(const string& strState, float inc = 0) override;
    };


    class StateIterator : public IStateIterator {
        IMPLEMENT_REFCOUNTER()
        Sequencer::CPtr _sequencer;
        Sequencer::sequencer_list_type::const_iterator _it;
    public:
        typedef explicit_ptr<StateIterator> Ptr;
        typedef explicit_ptr<const StateIterator> CPtr;
        StateIterator();
        StateIterator(const StateIterator& other);

        void assign(const Sequencer::CPtr& seq);
        bool hasNext() const override;
        IState::CPtr next() override;
        IState::CPtr peekNext() const override;
        int64_t size() const override;
        explicit_ptr<IStateIterator> clone() const override;
        //virtual IStateIterator* operator=(const IState::CPtr& container) = 0;
    };
}



#endif // GOAP_SEQUENCER_H
