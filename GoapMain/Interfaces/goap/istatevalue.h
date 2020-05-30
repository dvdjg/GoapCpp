#ifndef GOAP_ISTATEVALUE_H
#define GOAP_ISTATEVALUE_H

#include <cmath>
#include <string>
#include "common/irefcounter.h"
#include "ihashable.h"
#include "iclonable.h"
#include "istringvalue.h"
#include "explicit_ptr.h"

namespace goap
{

class IStateValue : public IStringValue, public IClonable, public IHashable, public virtual IRefCounter
{
public:
    using IStringValue::assign;
    using IStringValue::equals;

    typedef IStateValue class_type;
    typedef float value_type;
    typedef int64_t index_type;
    typedef explicit_ptr<IStateValue> Ptr;
    typedef explicit_ptr<const IStateValue> CPtr;

    class New : public class_type::Ptr {
        typedef class_type::Ptr parent_type;
    public:
        New();
        New(parent_type::element_type* pVal);
        New(const parent_type &other);
        New(const string &str);
        New(const char *sz);
        New(const initializer_list<float>& list);
        New(float val);
        New(int64_t val);
        New(int val);
    };

    class CNew : public class_type::CPtr {
        typedef class_type::CPtr parent_type;
    public:
        CNew();
        CNew(const class_type::Ptr &other);
        CNew(const parent_type::element_type* pVal);
        CNew(const parent_type &other);
        CNew(const string &str);
        CNew(const char *sz);
        CNew(const initializer_list<float>& list);
        CNew(float val);
        CNew(int64_t val);
        CNew(int val);
    };

    virtual int64_t size() const = 0; ///< From 0 to 1000
    virtual bool empty() const { return size() == 0; }
    virtual void resize(int64_t len) = 0;
    virtual float at(float idx) const = 0;
    virtual float at(int64_t idx) const = 0;
    virtual float at(int idx) const { return at(static_cast<int64_t>(idx));}
    virtual void put(float idx, float value) = 0;
    virtual void put(int64_t idx, float value) = 0;
    virtual void put(int idx, float value) { put(static_cast<int64_t>(idx), value);}
    virtual void putAll(float value) = 0;
    virtual IStringValue* assign(const IStateValue::CNew &other) = 0;
    virtual IStringValue* assign(const std::initializer_list<float> &list) = 0;
    virtual void interpolateFrom(const IStateValue::CNew &other) = 0;
    virtual float cosineDistance(const IStateValue::CNew &other, float *pThisModule = nullptr, float *pOthersModule = nullptr) const = 0;
    virtual float levensteinDistance(const IStateValue::CNew &other) const = 0;
    virtual bool equals(const IStateValue::CNew &other) const = 0;
    virtual bool equals(const std::initializer_list<float> &list) const = 0;
    virtual bool lessThan(const IStateValue::CNew &other) const = 0;

    virtual void add(const IStateValue::CNew &other) = 0;
    virtual void mul(const IStateValue::CNew &other) = 0;
    virtual void and_logic(const IStateValue::CNew &other) = 0;
    virtual void or_logic (const IStateValue::CNew &other) = 0;
    virtual void add(float other) = 0;
    virtual void mul(float other) = 0;
    virtual void and_logic(bool other) = 0;
    virtual void or_logic (bool other) = 0;

    virtual void clear() = 0;

    inline float at(uint64_t idx) const {
        return at(int64_t(idx));
    }

    inline void put(uint64_t idx, float value) {
        put(int64_t(idx), value);
    }

    inline float operator[](float idx) const {
        return at(idx);
    }

    inline float operator[](int64_t idx) const {
        return at(idx);
    }

    inline float operator[](uint64_t idx) const {
        return at(idx);
    }

    inline operator float() const {
        return at(0LL);
    }

    inline bool operator < (const IStateValue::CPtr &other) const {
        return lessThan(other);
    }

    inline bool isInt() const {
        bool ret = false;
        if (size() == 1) {
            float value = at(0LL);
            float whole, fractional;
            fractional = std::modf(value, &whole);
            ret = fractional == 0.f;
        }
        return ret;
    }

};

inline bool operator ==(const IStateValue& a, const IStateValue& b) {
    return a.equals(IStateValue::CPtr(&b));
}

inline bool operator ==(const IStateValue& a, const std::string &other) {
    return a.equals(other);
}

inline bool operator ==(const IStateValue& a, const char *other) {
    return a.equals(other);
}

inline bool operator ==(const IStateValue& a, const std::initializer_list<float> &other) {
    return a.equals(other);
}

inline bool operator ==(const IStateValue& a, float other) {
    return a.equals({other});
}

inline bool operator ==(const IStateValue& a, bool other) {
    bool isFalse = a.size() == 0 || a.equals({0.f});
    return other ? !isFalse : isFalse;
}

inline bool operator ==(const IStateValue& a, int64_t other) {
    return a.equals({float(other)});
}

inline bool operator !=(const IStateValue& a, const IStateValue& b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, const std::string& b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, const char *b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, const std::initializer_list<float>& b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, float b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, int64_t b) {
    return !(a == b);
}

inline bool operator !=(const IStateValue& a, bool b) {
    return !(a == b);
}

inline IStateValue& operator +=(IStateValue& a, const IStateValue& b) {
    a.add(IStateValue::CNew(&b));
    return a;
}

inline IStateValue& operator *=(IStateValue& a, const IStateValue& b) {
    a.mul(IStateValue::CNew(&b));
    return a;
}

inline bool operator <(const IStateValue::CPtr& a, const IStateValue::CPtr& b) {
    return *a < *b;
}

inline bool operator <(const IStateValue::Ptr& a, const IStateValue::CPtr& b) {
    return *a < *b;
}

inline bool operator <(const IStateValue::CNew& a, const IStateValue::CNew& b) {
    return *a < *b;
}

inline bool operator <(const IStateValue::New& a, const IStateValue::New& b) {
    return *a < *b;
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

template <>
struct hash<IStateValue::CNew>
{
    std::size_t operator()(const IStateValue::CNew &k) const {
        return k->hash();
    }
};

template <>
struct hash<IStateValue::New>
{
    std::size_t operator()(const IStateValue::New &k) const {
        return k->hash();
    }
};

template<>
struct equal_to<IStateValue::Ptr>
{
    bool operator()(const IStateValue::Ptr &data1, const IStateValue::CPtr &data2) const {
        return data1->equals(data2);
    }
};
template<>
struct equal_to<IStateValue::CPtr>
{
    bool operator()(const IStateValue::CPtr &data1, const IStateValue::CPtr &data2) const {
        return data1->equals(data2);
    }
};

template<>
struct equal_to<IStateValue::CNew>
{
    bool operator()(const IStateValue::CNew &data1, const IStateValue::CPtr &data2) const {
        return data1->equals(data2);
    }
};

template<>
struct equal_to<IStateValue::New>
{
    bool operator()(const IStateValue::New &data1, const IStateValue::CPtr &data2) const {
        return data1->equals(data2);
    }
};

template<>
struct less<IStateValue::Ptr>
{
    bool operator()(IStateValue::Ptr const& lhs, IStateValue::Ptr const& rhs) {
        return lhs->lessThan(rhs);
    }
};

template<>
struct less<IStateValue::CPtr>
{
    bool operator()(IStateValue::CPtr const& lhs, IStateValue::CPtr const& rhs) {
        return lhs->lessThan(rhs);
    }
};

template<>
struct less<IStateValue::New>
{
    bool operator()(IStateValue::New const& lhs, IStateValue::New const& rhs) {
        return lhs->lessThan(rhs);
    }
};

template<>
struct less<IStateValue::CNew>
{
    bool operator()(const IStateValue::CNew & lhs, const IStateValue::CNew & rhs) {
        const IStateValue* p = lhs.get();
        return p->lessThan(rhs);
    }
};
}


#endif // GOAP_ISTATEVALUE_H
