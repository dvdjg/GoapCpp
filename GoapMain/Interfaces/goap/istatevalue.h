#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H

#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"
#include "iclonable.h"
#include "istringvalue.h"

namespace goap
{

class IStateValue : public IStringValue, public IClonable, public virtual IRefCounter
{
public:
    typedef explicit_ptr<IStateValue> Ptr;
    typedef explicit_ptr<const IStateValue> CPtr;

    inline static bool floatEqual(float x, float y)
    {
        float diff = std::abs(x - y);
        return diff <= FLT_EPSILON * std::abs(x + y) * 2 || diff < FLT_MIN;
    }

    //virtual bool isNumeric() const = 0;
    virtual std::size_t size() const = 0; ///< From 0 to 1000
    virtual void resize(std::size_t len) = 0;
    virtual float atF(float idx) const = 0;
    virtual void setAtF(float idx, float value) = 0;
    virtual float at(size_t idx) const = 0;
    virtual void setAt(size_t idx, float value) = 0;
    virtual void assign(const IStateValue::CPtr &other) = 0;
    virtual void interpolateFrom(const IStateValue::CPtr &other) = 0;
    virtual float cosineDistance(const IStateValue::CPtr &other) const = 0;
    virtual bool equal(const IStateValue::CPtr &other) const
    {
        bool ret(other);
        if (ret)
        {
            std::size_t thisSize = size();
            ret = thisSize == other->size();
            for (std::size_t i = 0; ret && i < thisSize; ++i) {
                ret = floatEqual(at(i), other->at(i));
            }
        }
        return ret;
    }

    virtual std::size_t hash() const = 0;

    inline float operator[](float idx) const
    {
        return atF(idx);
    }
    inline float operator[](size_t idx) const
    {
        return at(idx);
    }
    //virtual float & operator[](float idx) = 0;
};

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

