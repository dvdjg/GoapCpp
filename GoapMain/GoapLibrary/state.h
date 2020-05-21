#ifndef GOAP_STATE_H
#define GOAP_STATE_H

#include <unordered_map>
#include "refcounter.h"
#include "goap/istate.h"

namespace goap
{

class State : public IState
{
    IMPLEMENT_REFCOUNTER()

public:
    typedef std::unordered_map<IStateValue::CPtr, IStateValue::Ptr> data_type;

protected:
    data_type _data;
    float _coste = 1;
    mutable std::size_t _cachedHash = 0;

private:
    void touch();

public:
    State();
    State(const State &other);
    State(const IState::CPtr &other);    
    State(std::initializer_list<data_type::value_type> list);

    void clear();

    //data_type::const_iterator begin() const override;
    //data_type::const_iterator end() const override;
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
};

}

#endif // GOAP_STATE_H
