#ifndef GOAP_ISTATEVALUE_H
#define GOAP_ISTATEVALUE_H

#include <cmath>
#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "iclonable.h"
#include "istringvalue.h"
#include "goap/ihashable.h"

namespace goap
{

class IStateValue : public virtual IStringValue, public virtual IClonable, public virtual IHashable
{
public:
    using IStringValue::assign;
    using IStringValue::equals;

    typedef float value_type;
    typedef intptr_t index_type;
    typedef explicit_ptr<IStateValue> Ptr;
    typedef explicit_ptr<const IStateValue> CPtr;

    //virtual bool isNumeric() const = 0;
    virtual intptr_t size() const = 0; ///< From 0 to 1000
    virtual void resize(intptr_t len) = 0;
    virtual float at(float idx) const = 0;
    virtual float at(intptr_t idx) const = 0;
    virtual void put(float idx, float value) = 0;
    virtual void put(intptr_t idx, float value) = 0;
    virtual void putAll(float value) = 0;
    virtual void assign(const IStateValue::CPtr &other) = 0;
    virtual void assign(const std::initializer_list<float> &list) = 0;
    virtual void interpolateFrom(const IStateValue::CPtr &other) = 0;
    virtual float cosineDistance(const IStateValue::CPtr &other, float *pThisModule = nullptr, float *pOthersModule = nullptr) const = 0;
    virtual bool equals(const IStateValue::CPtr &other) const = 0;
    virtual bool equals(const std::initializer_list<float> &list) const = 0;

    virtual void add(const IStateValue::CPtr &other) = 0;
    virtual void mul(const IStateValue::CPtr &other) = 0;
    virtual void and_logic(const IStateValue::CPtr &other) = 0;
    virtual void or_logic (const IStateValue::CPtr &other) = 0;
    virtual void add(float other) = 0;
    virtual void mul(float other) = 0;
    virtual void and_logic(bool other) = 0;
    virtual void or_logic (bool other) = 0;

    virtual void clear() = 0;
    //virtual std::size_t hash() const = 0;

    inline float at(int idx) const {
        return at(intptr_t(idx));
    }

    inline void put(int idx, float value) {
        put(intptr_t(idx), value);
    }

    inline float at(unsigned idx) const {
        return at(intptr_t(idx));
    }

    inline void put(unsigned idx, float value) {
        put(intptr_t(idx), value);
    }

    inline float operator[](float idx) const {
        return at(idx);
    }

    inline float operator[](intptr_t idx) const {
        return at(idx);
    }

    inline float operator[](int idx) const {
        return at(idx);
    }

    inline float operator[](unsigned idx) const {
        return at(idx);
    }

    inline operator float() const {
        return at(0);
    }
};

inline bool operator ==(const IStateValue& a, const IStateValue& b) {
    return a.equals(IStateValue::CPtr(&b));
}

inline bool operator ==(const IStateValue& a, const std::string &other) {
    return a.equals(other);
}

inline bool operator ==(const IStateValue& a, const std::initializer_list<float> &other) {
    return a.equals(other);
}

inline bool operator ==(const IStateValue& a, float other) {
    return a.equals({other});
}

inline bool operator ==(const IStateValue& a, bool other) {
    return a.equals({other ? 1.f : 0.f});
}

inline bool operator ==(const IStateValue& a, int other) {
    return a.equals({float(other)});
}

inline bool operator !=(const IStateValue& a, const IStateValue& b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, const std::string& b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, const std::initializer_list<float>& b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, float b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, int b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, bool b) {
    return !(a == b);
}

inline IStateValue& operator +=(IStateValue& a, const IStateValue& b) {
    a.add(IStateValue::CPtr(&b));
    return a;
}

inline IStateValue& operator *=(IStateValue& a, const IStateValue& b) {
    a.mul(IStateValue::CPtr(&b));
    return a;
}


}

namespace std
{
using namespace goap;

template <>
struct hash<IStateValue::CPtr>
{
    std::size_t operator()(const IStateValue::CPtr &k) const {
        return k->hash();
    }
};

template <>
struct hash<IStateValue::Ptr>
{
    std::size_t operator()(const IStateValue::Ptr &k) const {
        return k->hash();
    }
};

template<>
struct equal_to<IStateValue::CPtr>
{
    bool operator()(const IStateValue::CPtr &data1, const IStateValue::CPtr &data2) const {
        return data1->equals(data2);
    }
};

}


#endif // GOAP_ISTATEVALUE_H
