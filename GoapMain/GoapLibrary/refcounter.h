#ifndef GOAP_REFCOUNTER_H
#define GOAP_REFCOUNTER_H

#include <boost/intrusive_ptr.hpp>
#include <atomic>
#include "hasmember.h"
#include "instancedeleter.h"

#define IMPLEMENT_REFCOUNTER_FUNCTIONS(_refcount) \
    protected: \
    void suicide() { instanceDeleter(this); } \
    int loadRef() const { return _refcount.load(std::memory_order_relaxed); } \
    void storeRef(int newValue) const { _refcount.store(newValue, std::memory_order_relaxed); } \
    int addRef() const { return ++_refcount; } \
    int releaseRef() const { return --_refcount; }

#define IMPLEMENT_REFCOUNTER_FUNCTIONS_OVERRIDE(_refcount) \
    protected: \
    void suicide() override { instanceDeleter(this); } \
    int loadRef() const override { return _refcount.load(std::memory_order_relaxed); } \
    void storeRef(int newValue) const { _refcount.store(newValue, std::memory_order_relaxed); } \
    int addRef() const override { return ++_refcount; } \
    int releaseRef() const override { return --_refcount; }

#define IMPLEMENT_REFCOUNTER() \
    private: \
    mutable std::atomic<int> _refcount{0}; \
    IMPLEMENT_REFCOUNTER_FUNCTIONS_OVERRIDE(_refcount)

/** @brief GOAP_DECLARE_SHARED_PARENT macro
    Use this macro to bypass the reference counter calls to a parent implementor
*/
#define IMPLEMENT_REFCOUNTER_PARENT(parent)  \
    protected: \
    void suicide() override { parent::suicide(); } \
    int loadRef() const override { return parent::loadRef(); } \
    void storeRef(int count) const { parent::storeRef(count); } \
    int addRef() const override { return parent::addRef(); } \
    int releaseRef() const override { return parent::releaseRef(); }

#define IMPLEMENT_REFCOUNTER_DUMMY \
    protected: \
    void suicide() { } \
    int loadRef() const { return -1; } \
    void storeRef(int) const {  } \
    int addRef() const { return -1; } \
    int releaseRef() const { return -1; }

#endif // GOAP_REFCOUNTER_H
