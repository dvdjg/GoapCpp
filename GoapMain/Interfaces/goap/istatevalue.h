#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H
#include <memory>
#include <string>
#include "common/irefcounter.h"
#include "../half/half.h"

namespace goap
{
class IStateValue;
typedef std::shared_ptr<IStateValue> PtrIValue;
typedef std::shared_ptr<const IStateValue> CPtrIValue;

class IStateValue : public IRefCounter
{
public:
    //virtual bool isNumeric() const = 0;
    virtual PtrIValue clone() const = 0;
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
  struct hash<PtrIValue>
  {
    std::size_t operator()(const PtrIValue& k) const
    {
      return k->hash();
    }
  };

  template<>
  struct equal_to<PtrIValue>
  {
      bool operator() (const PtrIValue & data1, const PtrIValue & data2) const
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

