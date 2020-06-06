#ifndef GOAP_STATE_H
#define GOAP_STATE_H

#include <unordered_map>
#include "refcounter.h"
#include "goap/istate.h"

namespace goap
{

using namespace std;

class State : public IState
{
    IMPLEMENT_REFCOUNTER()

public:
    typedef explicit_ptr<State> Ptr;
    typedef explicit_ptr<const State> CPtr;
    typedef unordered_map<IStateValue::CPtr, IStateValue::Ptr> data_type;

protected:
    data_type _data;
    map_state_iterator_type _stateIterator;
    float _coste = 1;
    mutable size_t _cachedHash = 0;

private:
    void touch();

public:
    State();
    State(const State &other);
    State(const IState::CPtr &other);    
    State(initializer_list<data_type::value_type> list);

    void clear();

    const data_type& data() const { return _data; }
    const data_type& data() { return _data; }

    list<IStateValue::CPtr> keys() const override;
    IState* remove(const IStateValue::CNew &key) override;
    pair_value at(int64_t idx) const override;
    IStateValue* at(const IStateValue::CNew &key) const override;
    IStateValue& atRef(const IStateValue::CNew &key) const override;
    IState* put(const IStateValue::CNew &key, const IStateValue::New &value) override;
    IState* add(const IStateValue::CNew &key, const IStateValue::New &value) override;
    IState* mul(const IStateValue::CNew &key, const IStateValue::New &value) override;
    IState* addCost(float c) override;
    IState* mulCost(float c) override;
    int64_t size() const override;
    bool equals(const IHashable::CPtr &other) const override;
    bool equals(const IState::CPtr &other) const override;
    float cost() const override;
    IState* cost(float c) override;
    IState* assign(const State &other);
    IState* assign(const IState::CPtr &other) override;
    IState* assign(const map_value2value_type &map_string_float) override;
    IState* merge(const IState::CNew& other) override;
    explicit_ptr<IStateValueIterator> iterator() const override;
    IClonable::Ptr clone() const override;

    // IStringPrintable interface
public:
    std::string toDebugString() const override;
    std::string toString() const override;
    ostream& toOstream(ostream& os) const override;

    // IHashable interface
public:
    std::size_t hash() const override;

    // IState interface
public:
    bool empty() const override;

    void putStateIterator(const string& name, const explicit_ptr<IStateIterator>& satateIterator) override;
    explicit_ptr<IStateIterator> getStateIterator(const string& name) const override;
    void flashSequences() override;
    bool flashSequence(const string& name, bool clearOldState) override;
};


class StateValueIterator : public IStateValueIterator {
    IMPLEMENT_REFCOUNTER()

    State::CPtr _state;
    State::data_type::const_iterator _it;
public:
    typedef explicit_ptr<StateValueIterator> Ptr;
    typedef explicit_ptr<const StateValueIterator> CPtr;

public:
    StateValueIterator() {}
    StateValueIterator(const StateValueIterator& other) : _state(other._state), _it(other._it) {
    }
    StateValueIterator(const State::Ptr& other) : _state(other), _it(_state->data().cbegin()) {
    }
    void clear() {
        _state.reset();
        _it = {};
    }
    void assign(const State::CPtr& other) {
        _state = other;
        _it = _state->data().cbegin();
    }
    void assign(const IState::CPtr& other) {
        auto ptr = dynamic_pointer_cast<const State>(other);
        assign(ptr);
    }
    bool hasNext() const override {
        return _it != _state->data().cend();
    }
    IState::pair_value next() override {
        IState::pair_value ret = std::make_pair(_it->first, _it->second);
        ++_it;
        return ret;
    }
    IState::pair_value peekNext() override {
        IState::pair_value ret = std::make_pair(_it->first, _it->second);
        return ret;
    }
    int64_t size() const override {
        return _state->size();
    }
    virtual IStateValueIterator* operator=(const IState::CPtr &other) override {
        assign(other);
        return this;
    }

};

}

#endif // GOAP_STATE_H
