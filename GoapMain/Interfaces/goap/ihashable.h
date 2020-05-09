#ifndef GOAP_IHASHABLE_H
#define GOAP_IHASHABLE_H

#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{

class IHashable : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IHashable> Ptr;
    typedef explicit_ptr<const IHashable> CPtr;

    virtual std::size_t hash() const = 0;
    virtual bool equal(const IHashable::CPtr &other) const = 0;
};
//std::is_base_of

inline bool operator ==(const IHashable& a, const IHashable& b) {
    return a.equal(IHashable::CPtr(&b));
}

inline bool operator !=(const IHashable& a, const IHashable& b) {
    return !(a == b);
}

}

namespace std
{
using namespace goap;

template <>
struct hash<IHashable::CPtr>
{
    std::size_t operator()(const IHashable::CPtr &k) const {
        return k->hash();
    }
};

template <>
struct hash<IHashable::Ptr>
{
    std::size_t operator()(const IHashable::Ptr &k) const {
        return k->hash();
    }
};

template<>
struct equal_to<IHashable::CPtr>
{
    bool operator()(const IHashable::CPtr &data1, const IHashable::CPtr &data2) const {
        return data1->equal(data2);
    }
};

}

#endif // GOAP_IHASHABLE_H
