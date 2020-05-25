#ifndef GOAP_STATEVALUE_H
#define GOAP_STATEVALUE_H

#include <vector>
#include <memory>
#include "refcounter.h"
#include "goap/istatevalue.h"
#include "newptr.h"

namespace goap
{

using namespace std;

class StateValue : public IStateValue
{
    IMPLEMENT_REFCOUNTER()

protected:
    typedef vector<float> container_type;
    container_type _data;
    mutable std::size_t _cachedHash = 0;

#ifdef GOAP_DEBUG
    string _strDebug;
#endif

public:
    typedef explicit_ptr<StateValue> Ptr;
    typedef explicit_ptr<const StateValue> CPtr;

    StateValue();
    StateValue(const StateValue & other);
    StateValue(const IStateValue::CNew &other);
    StateValue(const string &str);
    StateValue(const char* other);
    StateValue(initializer_list<float> list);

    int64_t size() const override;
    void resize(int64_t len) override;
    float at(float idx) const override;
    void put(float idx, float value) override;
    float at(int64_t idx) const override;
    void put(int64_t idx, float value) override;
    void putAll(float value) override;
    void interpolateFrom(const IStateValue::CNew &other) override;
    float cosineDistance(const IStateValue::CNew &other, float *pThisModule = nullptr, float *pOthersModule = nullptr) const override;
    float levensteinDistance(const IStateValue::CNew &other) const override;
    IStringValue* fromString(const string &str) override;
    IStringValue* assign(const IStateValue::CNew &other) override;
    IStringValue* assign(const StateValue &other);
    IStringValue* assign(const string &other) override;
    IStringValue* assign(const char* other) override;
    IStringValue* assign(const initializer_list<float> &list) override;
    bool equals(const IStateValue::CNew &other) const override;
    bool equals(const IHashable::CPtr &other) const override;
    bool equals(const string &other) const override;
    bool equals(const char *str) const override;
    bool equals(const initializer_list<float> &other) const override;
    string toDebugString() const override;
    string toString() const override;
    ostream& toOstream(ostream& os) const override;
    size_t hash() const override;

    void clear() override;
    IClonable::Ptr clone() const override;

    // IStateValue interface
public:
    void add(const IStateValue::CNew &other) override;
    void mul(const IStateValue::CNew &other) override;
    void and_logic(const IStateValue::CNew &other) override;
    void or_logic(const IStateValue::CNew &other) override;
    void add(float other) override;
    void mul(float other) override;
    void and_logic(bool other) override;
    void or_logic(bool other) override;
private:
    void touch();
};

}
#endif // GOAP_STATEVALUE_H
