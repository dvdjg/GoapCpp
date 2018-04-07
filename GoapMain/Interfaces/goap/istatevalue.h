#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H
#include <memory>
#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{
class IStateValue;
typedef explicit_ptr<IStateValue> PtrIStateValue;
typedef explicit_ptr<const IStateValue> CPtrIStateValue;

class IStringValue : public virtual IRefCounter
{
public:
    virtual void fromString(const std::string &str) = 0;
    virtual std::string toString() const = 0;
};

class IStateValue : public IStringValue
{
public:
    //virtual bool isNumeric() const = 0;
    virtual PtrIStateValue clone() const = 0;
    virtual std::size_t size() const = 0; ///< From 0 to 1000
    virtual void resize(std::size_t len) = 0;
    virtual float at(float idx = 0) const = 0;
    virtual void setAt(float idx, float value) = 0;
    virtual void interpolateFrom(const IStateValue *other) = 0;
    virtual float cosineDistance(const IStateValue *other) const = 0;

    virtual std::size_t hash() const = 0;

    inline float operator[](float idx) const
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
struct hash<PtrIStateValue>
{
    std::size_t operator()(const PtrIStateValue &k) const
    {
        return k->hash();
    }
};

template<>
struct equal_to<PtrIStateValue>
{
    bool operator()(const PtrIStateValue &data1, const PtrIStateValue &data2) const
    {
        bool ret = !data1 && !data2;
        if (!ret)
        {
            ret = data1 && data2 && data1->size() == data2->size();
            if (ret)
            {

            }
        }
        return ret;
    }
};
}


#endif // ISTATEVALUE_H

