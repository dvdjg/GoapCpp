#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H

#include <string>
#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{

class IClonable : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IClonable> Ptr;
    typedef explicit_ptr<const IClonable> CPtr;
    virtual Ptr clone() const = 0;
};

class IStringValue : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IStringValue> Ptr;
    typedef explicit_ptr<const IStringValue> CPtr;
    virtual void fromString(const std::string &str) = 0;
    virtual std::string toString() const = 0;
};

class IStateValue : public IStringValue, public IClonable, public virtual IRefCounter
{
public:
    typedef explicit_ptr<IStateValue> Ptr;
    typedef explicit_ptr<const IStateValue> CPtr;

    //virtual bool isNumeric() const = 0;
    virtual std::size_t size() const = 0; ///< From 0 to 1000
    virtual void resize(std::size_t len) = 0;
    virtual float at(float idx = 0) const = 0;
    virtual void setAt(float idx, float value) = 0;
    virtual void interpolateFrom(CPtr other) = 0;
    virtual float cosineDistance(CPtr other) const = 0;

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
struct hash<IStateValue::Ptr>
{
    std::size_t operator()(const IStateValue::Ptr &k) const
    {
        return k->hash();
    }
};

template<>
struct equal_to<IStateValue::Ptr>
{
    bool operator()(const IStateValue::Ptr &data1, const IStateValue::Ptr &data2) const
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

