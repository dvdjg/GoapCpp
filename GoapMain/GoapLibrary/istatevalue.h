#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H
#include <memory>
#include <string>
#include "iroot.h"
#include "../half/half.h"

namespace goap
{
class IStateValue;
typedef std::shared_ptr<IStateValue> ptr_value;
typedef std::shared_ptr<const IStateValue> cptr_value;

class IStateValue : public IRoot
{
public:
    //virtual bool isNumeric() const = 0;
    virtual ptr_value clone() const = 0;
    virtual std::size_t size() const = 0; ///< From 0 to 1000
    virtual void resize(std::size_t len) = 0;
    virtual float at(float idx = 0) const = 0;
    virtual void setAt(float idx, float value) = 0;
    virtual void interpolateFrom(const IStateValue * other) = 0;
    virtual float cosineDistance(const IStateValue * other) const = 0;
    virtual void fromString(const std::u16string & str) = 0;
    virtual std::u16string toString() const = 0;
    virtual std::size_t hash() const;

    inline float operator[](float idx) const { return at(idx); }
    //virtual float & operator[](float idx) = 0;
};
}

namespace std {
using namespace goap;

  template <>
  struct hash<ptr_value>
  {
    std::size_t operator()(const ptr_value& k) const
    {
      return k->hash();
    }
  };

  template<>
  struct equal_to<ptr_value>
  {
      bool operator() (const ptr_value & data1, const ptr_value & data2) const
      {
          bool ret = !data1 && !data2;
          if (!ret) {
              ret = data1 && data2 && data1->size() == data2->size();
              if (ret) {

              }
          }
          return ret;
      }
  };
}


#endif // ISTATEVALUE_H

