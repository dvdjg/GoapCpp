#include "path.h"
#include "newptr.h"

namespace goap
{

Path::Path(IPlanningAction::CPtr action_, IPath::Ptr parent_, float cost_) :
    _action(action_), _parent(parent_), _cost(cost_) {
}

void Path::clear() {
    _action.reset();
    _parent.reset();
    _cost = 0;
    _initialState.reset();
    _finalState.reset();
}

IPath::Ptr Path::parent() const {
    return _parent;
}

void Path::setParent(IPath::Ptr parent_) {
    _parent = parent_;
}

float Path::cost() const {
    return _cost;
}

void Path::setCost(float cost_) {
    _cost = cost_;
}

float Path::distance() const {
    return _parent ? _cost : _cost - _parent->cost();
}

IPlanningAction::CPtr Path::action() const {
    return _action;
}

void Path::setAction(IPlanningAction::CPtr action_) {
    _action = action_;
}

IPath::Ptr Path::addChild(IPlanningAction::Ptr node, float cost_) {
    IPath::Ptr p = NewPtr<IPath>({}, node, this, _cost + cost_);
    return p;
}

IState::Ptr Path::executeFromRoot(IState::CPtr initialState) {
    IState::Ptr finalState;
    if (_initialState && _initialState->equal(initialState)) {
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
    for (Path::CPtr path = this; path; path = path->_parent) {
        nActions++;
    }
    return nActions;
}

void Path::getActions(std::list<IPlanningAction::CPtr> &actions) const {
    for (Path::CPtr path = this; path; path = path->_parent) {
        actions.push_back(path->_action);
    }
}

void Path::getStates(IState::CPtr initialState, std::list<IState::CPtr> &states) {
    for (Path::Ptr path = this; path; path = path->_parent) {
        states.push_back(path->executeFromRoot(initialState));
    }
}
}
