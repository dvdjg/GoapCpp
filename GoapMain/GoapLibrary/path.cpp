#include <sstream>
#include "path.h"
#include "newptr.h"

namespace goap
{

Path::Path() : _cost(1) {}

Path::Path(IPlanningAction::CPtr action_, IPath::Ptr parent_, float cost_) :
    _action(action_), _parent(parent_), _cost(cost_) {
}

void Path::clear() {
    _action.reset();
    _parent.reset();
    _cost = 1;
    _initialState.reset();
    _finalState.reset();
}

IPath::Ptr Path::parent() const {
    return _parent;
}

void Path::parent(IPath::Ptr parent_) {
    _parent = parent_;
}

float Path::cost() const {
    return _cost;
}

void Path::setCost(float cost_) {
    _cost = cost_;
}

float Path::distance() const {
    return !_parent ? _cost : _cost - _parent->cost();
}

IPlanningAction::CPtr Path::action() const {
    return _action;
}

void Path::action(IPlanningAction::CPtr action_) {
    _action = action_;
}

IPath::Ptr Path::addChild(IPlanningAction::Ptr node, float cost_) {
    IPath::Ptr p = NewPtr<IPath>({}, node, IPath::Ptr(this), _cost + cost_);
    return p;
}

IState::Ptr Path::executeFromRoot(IState::CPtr initialState) {
    IState::Ptr finalState;
    if (_initialState && _initialState->equals(initialState)) {
        finalState = _finalState;
    } else {
        IState::CPtr state = (!_parent) ? initialState : _parent->executeFromRoot(initialState);
        finalState = _action->execute(state);
        _initialState = initialState;
        _finalState = finalState;
    }
    return finalState;
}

size_t Path::getActionCount() {
    size_t nActions = 0;
    for (IPath::CPtr path = this; path; path = path->parent()) {
        nActions++;
    }
    return nActions;
}

std::list<IPlanningAction::CPtr>& Path::getActions(std::list<IPlanningAction::CPtr> &actions) const {
    for (IPath::CPtr path = this; path; path = path->parent()) {
        actions.push_back(path->action());
    }
    return actions;
}

std::list<IState::CPtr> & Path::getStates(IState::CPtr initialState, std::list<IState::CPtr> &states) {
    for (IPath::Ptr path = this; path; path = path->parent()) {
        states.push_back(path->executeFromRoot(initialState));
    }
    return states;
}

string Path::toDebugString() const
{
    std::stringstream ss;
    ss << "[";
    const char *sz = "";
    for (IPath::CPtr path = this; path; path = path->parent()) {
        ss << sz << *path->action() << ":" << path->cost();
        sz = ", ";
    }
    ss << "]" << ends;
    string str = ss.str();
    return str;
}

string Path::toString() const
{
    return toDebugString();
}

}

