#include <sstream>
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
    IState::Ptr s = dynamic_pointer_cast<IState>(state->clone());
    s->cost(_time);
    IState::CPtr p = s; //  make_pair(_time, state);
    _lstStates.push_back(p);
}

IState::CPtr Sequencer::pullState() {
    IState::CPtr ret = _lstStates.front();
    _lstStates.pop_front();
    return ret;
}

IState::CPtr Sequencer::peekState() const {
    IState::CPtr ret = _lstStates.front();
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
    return toString();
}

std::string Sequencer::toString() const
{
    stringstream ss;
    toOstream(ss);
    return ss.str();
}

ostream& Sequencer::toOstream(ostream& os) const
{
    os << "[ ";
    for (auto it : this->_lstStates) {
        os << "{ time:" << it->cost() << ", state:" << *it << "}";
    }
    os << "]";
    return os;
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

IState::CPtr StateIterator::next() {
    IState::CPtr ret = *_it++;
    return ret;

}

IState::CPtr StateIterator::peekNext() const {
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