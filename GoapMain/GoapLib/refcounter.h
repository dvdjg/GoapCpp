#ifndef REFCOUNTER_H
#define REFCOUNTER_H

#include <boost/intrusive_ptr.hpp>
#include <boost/atomic.hpp>

#define IMPLEMENT_REFCOUNTER(Class) \
    public: \
    typedef boost::intrusive_ptr<Class> smart_pointer; \
    \
    private: \
    mutable boost::atomic<int> _refcount(0); \
    int load() { return this->_refcount; } \
    void suicide() { delete this; } \
    void addRef() \
    { \
        this->_refcount.fetch_add(1, boost::memory_order_relaxed); \
    } \
    void releaseRef() \
    { \
        if (this->_refcount.fetch_sub(1, boost::memory_order_release) == 1) { \
            boost::atomic_thread_fence(boost::memory_order_acquire); \
            this->suicide(); \
        } \
    }

#endif // REFCOUNTER_H
