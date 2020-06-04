#include "sequencer.h"
#include "newptr.h"

namespace goap {
using namespace std;

float Sequencer::getInputTime() {
    return _time;
}

void Sequencer::advanceInputTime(float ticks) {
    _time += ticks;
}

void Sequencer::pushState(const IState::CNew& state) {
    //IState::CPtr s = state;
    IState::pair_state p = make_pair(_time, state);
    _lstStates.push_back(p);
}

IState::pair_state Sequencer::pullState() {
    IState::pair_state ret = _lstStates.front();
    _lstStates.pop_front();
    return ret;
}

IState::pair_state Sequencer::peekState() const {
    IState::pair_state ret = _lstStates.front();
    return ret;
}

explicit_ptr<IStateIterator> Sequencer::iterator() {
    NewPtr<IStateIterator> itState;
    auto ptr = dynamic_pointer_cast<StateIterator>(itState);
    ptr->assign(this);
    return std::move(itState);
}

std::string Sequencer::toDebugString() const
{
    return std::string();
}

std::string Sequencer::toString() const
{
    return std::string();
}

ostream& Sequencer::toOstream(ostream& os) const
{
    // TODO: Insertar una instrucción "return" aquí
}

void Sequencer::pushString(const string& strState, float inc)
{
    for (auto ch : strState) {
        advanceInputTime(inc);
        IState::CPtr state = IState::CNew({ { ch, true } });
        pushState(state);
    }
}

StateIterator::StateIterator() {
}

StateIterator::StateIterator(const StateIterator& other) : _sequencer(other._sequencer), _it(other._it) {
}

void StateIterator::assign(const Sequencer::CPtr& seq) {
    _sequencer = seq;
    _it = seq->_lstStates.begin();
}

bool StateIterator::hasNext() const {
    return _it != _sequencer->_lstStates.end();
}

IState::pair_state StateIterator::next() {
    IState::pair_state ret = *_it++;
    return ret;

}

IState::pair_state StateIterator::peekNext() {
    return *_it;
}

int64_t StateIterator::size() const {
    return _sequencer->_lstStates.size();
}

explicit_ptr<IStateIterator> StateIterator::clone() const {
    NewPtr<IStateIterator> itState;
    auto ptr = dynamic_pointer_cast<StateIterator>(itState);
    ptr->_sequencer = _sequencer;
    ptr->_it = _it;
    return ptr;
}

}