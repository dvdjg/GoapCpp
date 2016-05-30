#ifndef ISTATEVALUE_H
#define ISTATEVALUE_H
#include <memory>
#include <string>
#include "iroot.h"
#include "../half/half.h"


class IStateValue : public IRoot
{
public:
    //virtual bool isNumeric() const = 0;
    virtual std::size_t length() const = 0; ///< From 0 to 1000
    virtual float at(float idx = 0) const = 0;
    virtual void setAt(float idx, float value) = 0;

    virtual void fromString(const std::u16string & str) = 0;
    virtual std::u16string toString() const = 0;
    virtual std::size_t hash() const;
};

typedef std::shared_ptr<IStateValue> ptr_value;

namespace std {

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
              ret = data1 && data2 && data1->length() == data2->length();
              if (ret) {

              }
          }
          return ret;
      }
  };
}


#endif // ISTATEVALUE_H

