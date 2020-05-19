#ifndef GOAP_ISTRINGVALUE_H
#define GOAP_ISTRINGVALUE_H

#include <string>
#include <ostream>
#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{
using namespace std;

class IStringPrintable : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IStringPrintable> Ptr;
    typedef explicit_ptr<const IStringPrintable> CPtr;
    virtual std::string toDebugString() const = 0;
    virtual std::string toString() const = 0;
    virtual ostream& toOstream(ostream& os) const = 0;
};

class IStringValue : public virtual  IStringPrintable
{
public:
    typedef explicit_ptr<IStringValue> Ptr;
    typedef explicit_ptr<const IStringValue> CPtr;
    virtual IStringValue* fromString(const std::string &str) = 0;
    virtual IStringValue* assign(const std::string &other) = 0;
    virtual IStringValue* assign(const char *other) = 0;
    virtual bool equals(const std::string &other) const = 0;
    virtual bool equals(const char *other) const = 0;
};

//template<class E, class Y, class T>
//inline static typename enable_if <is_convertible<T*, const IStringPrintable*>::value, std::basic_ostream<E, Y>>::type&
//operator<<(std::basic_ostream<E, Y>& os, const T& dt) {
//    return os << dt.toOstream(os);
//}

template<class E, class Y>
inline static std::basic_ostream<E, Y>& operator<<(std::basic_ostream<E, Y>& os, const IStringPrintable& dt) {
    return dt.toOstream(os);
}

}

#endif // GOAP_ISTRINGVALUE_H
