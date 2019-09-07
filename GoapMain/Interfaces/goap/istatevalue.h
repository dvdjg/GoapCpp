#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H

#include <cmath>
#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "iclonable.h"
#include "istringvalue.h"

#ifdef FLT_EPSILON
#define GOAP_FLT_EPSILON FLT_EPSILON
#else
#define GOAP_FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
#endif

#ifdef FLT_MIN
#define GOAP_FLT_MIN FLT_MIN
#else
#define GOAP_FLT_MIN          1.175494351e-38F        // min normalized positive value
#endif

namespace goap
{

class IStateValue : public IStringValue, public IClonable, public virtual IRefCounter
{
public:
    typedef float value_type;
    typedef explicit_ptr<IStateValue> Ptr;
    typedef explicit_ptr<const IStateValue> CPtr;

    inline static bool floatEqual(float x, float y)
    {
        float diff = std::abs(x - y);
        return diff <= GOAP_FLT_EPSILON * std::abs(x + y) * 2 || diff < GOAP_FLT_MIN;
    }

    //virtual bool isNumeric() const = 0;
    virtual std::size_t size() const = 0; ///< From 0 to 1000
    virtual void resize(std::size_t len) = 0;
    virtual float at(float idx) const = 0;
    virtual void setAt(float idx, float value) = 0;
    virtual float at(size_t idx) const = 0;
    virtual void setAt(size_t idx, float value) = 0;
    virtual float at(int idx) const = 0;
    virtual void setAt(int idx, float value) = 0;
    virtual void assign(const IStateValue::CPtr &other) = 0;
    virtual void assign(const std::string &other) = 0;
    virtual void assign(const std::initializer_list<float> &list) = 0;
    virtual void interpolateFrom(const IStateValue::CPtr &other) = 0;
    virtual float cosineDistance(const IStateValue::CPtr &other) const = 0;
    virtual bool equal(const IStateValue::CPtr &other) const = 0;
    virtual std::string toCharacterString() const = 0;

    virtual void clear() = 0;
    virtual std::size_t hash() const = 0;

    inline float operator[](float idx) const
    {
        return at(idx);
    }
    inline float operator[](size_t idx) const
    {
        return at(idx);
    }
};

inline bool operator ==(const IStateValue& a, const IStateValue& b)
{
    return a.equal(IStateValue::CPtr(&b));
}

inline bool operator !=(const IStateValue& a, const IStateValue& b)
{
    return !a.equal(IStateValue::CPtr(&b));
}

}

namespace std
{
using namespace goap;

template <>
struct hash<IStateValue::CPtr>
{
    std::size_t operator()(const IStateValue::CPtr &k) const
    {
        return k->hash();
    }
};
template <>
struct hash<IStateValue::Ptr>
{
    std::size_t operator()(const IStateValue::Ptr &k) const
    {
        return k->hash();
    }
};

template<>
struct equal_to<IStateValue::CPtr>
{
    bool operator()(const IStateValue::CPtr &data1, const IStateValue::CPtr &data2) const
    {
        return data1->equal(data2);
    }
};
}


#endif // ISTATEVALUE_H

