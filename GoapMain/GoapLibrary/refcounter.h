#ifndef GOAP_REFCOUNTER_H
#define GOAP_REFCOUNTER_H

#include <boost/intrusive_ptr.hpp>
#include <atomic>
#include "hasmember.h"
#include "instancedeleter.h"

#define IMPLEMENT_REFCOUNTER_FUNCTIONS(_refcount) \
    protected: \
    inline void suicide() { instanceDeleter(this); } \
    inline int loadRef() const { return _refcount.load(std::memory_order_relaxed); } \
    inline void storeRef(int newValue) const { _refcount.store(newValue, std::memory_order_relaxed); } \
    inline int addRef() const { return ++_refcount; } \
    inline int releaseRef() const { return --_refcount; }

#define IMPLEMENT_REFCOUNTER_FUNCTIONS_OVERRIDE(_refcount) \
    protected: \
    inline void suicide() override { instanceDeleter(this); } \
    inline int loadRef() const override { return _refcount.load(std::memory_order_relaxed); } \
    inline void storeRef(int newValue) const override { _refcount.store(newValue, std::memory_order_relaxed); } \
    inline int addRef() const override { return ++_refcount; } \
    inline int releaseRef() const override { return --_refcount; }

#define IMPLEMENT_REFCOUNTER() \
    private: \
    mutable std::atomic<int> _refcount{0}; \
    IMPLEMENT_REFCOUNTER_FUNCTIONS(_refcount)

/** @brief GOAP_DECLARE_SHARED_PARENT macro
    Use this macro to bypass the reference counter calls to a parent implementor
*/
#define IMPLEMENT_REFCOUNTER_PARENT(parent)  \
    public: \
    inline void suicide() { parent::suicide(); } \
    inline int loadRef() const { return parent::loadRef(); } \
    inline void storeRef(int count) const { parent::storeRef(count); } \
    inline int addRef() const { return parent::addRef(); } \
    inline int releaseRef() const { return parent::releaseRef(); }

#define IMPLEMENT_REFCOUNTER_DUMMY \
    public: \
    inline void suicide() { } \
    inline int loadRef() const { return -1; } \
    inline void storeRef(int) const {  } \
    inline int addRef() const { return -1; } \
    inline int releaseRef() const { return -1; }

#endif // GOAP_REFCOUNTER_H
