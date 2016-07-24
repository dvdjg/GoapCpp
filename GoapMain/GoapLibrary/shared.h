/** @brief Helpers to implement classes supporting Ptr<> smart pointers.

    This header is needed only to help implementing classes with reference
    counting support to satisfy the IRefCounter interface.
    @file byshared.h
*/

#ifndef GOAPSHAREDDATA_H
#define GOAPSHAREDDATA_H

#include <typeindex>
//#include <QAtomicInt>
#include "ptr.h"
#include "irefcounter.h"

namespace goap
{

/**
    @brief getClassName
    Helper template to retrieve a string representation of T.
    The string allocation happens only once.
    @return
*/
template<typename T>
QString getClassName() noexcept
{
    static QString ifName(::std::type_index(typeid(T)).name());
    return ifName;
}


/**
    @brief The has_goap_ref_counter class
    The internal 'value' will be true if T implements the needed functions
    for a effective reference counting through function members.
*/
template <typename T>
class has_goap_ref_counter
{
public:
    static bool const value =
        has_member_bool__releaseRef<T>::value
        && has_member_bool__addRef<T>::value
        && has_member_int__load<T>::value;
};

/**
    @brief The has_recyclable_goap_ref_counter class
    If the class implements the suicide() member function, and match
    has_goap_ref_counter, then it is fine for the recycling mechanism of object pools.
*/
template <typename T>
class has_recyclable_goap_ref_counter
{
public:
    static bool const value =
        has_member_void__suicide<T>::value && has_goap_ref_counter<T>::value;
};

// http://zchydem.enume.net/2010/01/19/qt-howto-private-classes-and-d-pointers/

/**
    @brief GOAP_DECLARE_PRIVATE macro
    Helper to implement the PIMPL idiom.
    Instead of using the Qt Q_DECLARE_PRIVATE macros family, here is pressented
    an alternative system with the addition of allowing to derive the
    class private data for reusing by the corresponding derived public classes.
*/

#define GOAP_DECLARE_PRIVATE(Class) \
protected: \
    inline Class##Private* d_func() { \
      return reinterpret_cast<Class##Private *>(dGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { \
      return reinterpret_cast<const Class##Private *>(dGetPtrHelper(d_ptr)); } \
    friend class Class##Private;


#define GOAP_SHARED_FUNCTIONS(refc) \
public: \
    virtual void suicide() { ::goap::PrivateFactory::instanceDeleter(this, 0); } \
    inline int load() const { return refc.load(std::memory_order_relaxed); } \
    inline void store(int newValue) { refc.store(newValue, std::memory_order_relaxed); } \
    inline bool addRef() const { return return ++refc != 0; } \
    inline bool releaseRef() const { return --refc != 0; }

#define GOAP_QSHARED_FUNCTIONS(refc) \
public: \
    virtual void suicide() { ::goap::PrivateFactory::instanceDeleter(this, 0); } \
    inline int load() const { return this->refc.load(); } \
    inline void store(int count) { this->refc.store(count); } \
    inline bool addRef() const { return this->refc.ref(); } \
    inline bool releaseRef() const { return this->refc.deref(); }

/**
    @brief GOAP_DECLARE_SHARED macro
    Declaration for implementing an IRefCounter-like interface (but not requiring it!!!).
    It also includes the suicide() function that is usefull for safe deletion:
    If you delete an object from one interface, you must provide a valid
    destructor for that interface, so no pure virtual classes can be used.
    With the suicide() function that is not necessary because it allways use
    the pointer "this" to delete the real objects instead a pointer to a interface.
*/
#define GOAP_DECLARE_SHARED  \
public: \
    mutable std::atomic_int stdAtomicInt; \
    GOAP_SHARED_FUNCTIONS(stdAtomicInt)

/** @brief GOAP_DECLARE_SHARED_PARENT macro
    Use this macro to bypass the reference counter calls to a parent implementor
*/
#define GOAP_DECLARE_SHARED_PARENT(parent)  \
public: \
    virtual void suicide() { parent::suicide(); } \
    inline int load() const { return parent::load(); } \
    inline void store(int count) { parent::store(count); } \
    inline bool addRef() const { parent::addRef(); } \
    inline bool releaseRef() const { return parent::releaseRef(); }

// This macro is the dummy version of GOAP_DECLARE_SHARED.
// It disables Ptr deletion possibility (a explicit delete is needed).
#define GOAP_DECLARE_DUMMY_SHARED \
public: \
    virtual void suicide() { } \
    inline int load() const { return -1; } \
    inline void store(int) {  } \
    inline bool addRef() const { return -1; } \
    inline bool releaseRef() const { return -1; }

/**
    @brief The GoapSharedData class
    A base class implementing the IRefCounter interface.
    This is the equivalent of QSharedData.
*/
class GoapSharedData : public IRefCounter
{
    GOAP_DECLARE_SHARED
public:
    inline GoapSharedData() { }
    inline virtual ~GoapSharedData() = default;
    inline GoapSharedData(const GoapSharedData &) { }

private:
    // using the assignment operator would lead to corruption in the ref-counting
    GoapSharedData &operator=(const GoapSharedData &);
};

/**
    @brief The WrapWithSharedData class
    Helper class to wrap a raw class for adding reference counting: Both the
    atomic counter and the IRefCounter functions.
*/
template <typename T>
class WrapWithSharedData : public T, virtual public IRefCounter
{
    GOAP_DECLARE_SHARED
public:
    inline virtual ~WrapWithSharedData() = default;
};

/**
    @brief The WrapQSharedData class
    Helper class to wrap a QSharedData derived class for adding
    the reference counting functions.
*/
template <typename T>
class WrapQSharedData : public T, virtual public IRefCounter
{
public:
    GOAP_QSHARED_FUNCTIONS(ref)
    inline virtual ~WrapQSharedData() = default;
};

}

#endif // GOAPSHAREDDATA_H
