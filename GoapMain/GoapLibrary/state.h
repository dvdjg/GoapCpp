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
    float _coste = 0;

public:
    State();
    State(const State &other);
    State(const IState::CPtr &other);    
    State(std::initializer_list<data_type::value_type> list);

    void clear();

    void remove(const IStateValue::CPtr &key) override;
    IStateValue& atRef(const IStateValue::CPtr &key) const override;
    IStateValue& atRef(const std::string &str) const override;
    IState::Ptr put(const IStateValue::CPtr &key, const IStateValue::Ptr &value) override;
    IState::Ptr put(const std::string &str, const IStateValue::Ptr &value) override;
    IState::Ptr put(const std::string &str, const std::string &value) override;
    IState::Ptr put(const std::string &str, std::initializer_list<float> list) override;
    IState::Ptr put(const std::string &str, float number) override;
    IState::Ptr add(const IStateValue::CPtr &key, const IStateValue::Ptr &value) override;
    IState::Ptr add(const std::string &str, const IStateValue::Ptr &value) override;
    IState::Ptr add(const std::string &str, const std::string &value) override;
    IState::Ptr add(const std::string &str, std::initializer_list<float> list) override;
    IState::Ptr add(const std::string &str, float number) override;
    IState::Ptr mul(const IStateValue::CPtr &key, const IStateValue::Ptr &value) override;
    IState::Ptr mul(const std::string &str, const IStateValue::Ptr &value) override;
    IState::Ptr mul(const std::string &str, const std::string &value) override;
    IState::Ptr mul(const std::string &str, std::initializer_list<float> list) override;
    IState::Ptr mul(const std::string &str, float number) override;
    intptr_t size() const override;
    void remove(const std::string &str) override;
    bool equals(const IHashable::CPtr &other) const override;
    bool equals(const IState::CPtr &other) const override;
    float cost() const override;
    void cost(float c) override;
    IState::Ptr addCost(float c) override;
    IState::Ptr mulCost(float c) override;
    void assign(const IState::CPtr &other) override;
    void assign(const State &other);
    void assign(const map_string_float_type &map_string_float) override;

    pair_value at(intptr_t idx) const override;

    // IClonable interface
public:
    IClonable::Ptr clone() const override;

    // IStringPrintable interface
public:
    std::string toDebugString() const override;
    std::string toString() const override;

    // IHashable interface
public:
    std::size_t hash() const override;
};

}

#endif // GOAP_STATE_H
