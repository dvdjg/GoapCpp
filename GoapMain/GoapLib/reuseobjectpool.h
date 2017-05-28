#ifndef REUSEOBJECTPOOL_H
#define REUSEOBJECTPOOL_H

#include <mutex>
#include <queue>
#include <memory>
#include <boost/intrusive_ptr.hpp>
#include "instancedeleter.h"

namespace goap
{

template<typename T>
class ReuseObjectPool
{
public:
    typedef std::queue<T *> pool_type;
    typedef typename pool_type::size_type size_type;
    typedef std::function <T*()> function_type;

    template<typename F>
    ReuseObjectPool(F &&func, size_type maxSize = 512) : _maxSize(maxSize), _func(std::forward<F>(func)) {}

    static ReuseObjectPool<T> *singleton()
    {
        static ReuseObjectPool<T> pool{[]()
        {
            return new T;
        }};
        return &pool;
        //return nullptr;
    }

    size_type maxSize() const
    {
        return _maxSize;
    }

    void setMaxSize(size_type maxSize)
    {
        _maxSize = maxSize;
    }

    T *create()
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!_pool.empty())
            {
                T *ret = _pool.front();
                _pool.pop();
                return ret;
            }
        }
        return _func();
    }

    void recycle(T *t)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_pool.size() < _maxSize)
            {
                _pool.push(t);
                return;
            }
        }
        instanceDeleter(t);
    }

private:
    std::mutex _mutex;
    pool_type _pool;
    size_type _maxSize;
    function_type _func;
};

template<typename P>
class Recyclable : public P
{
public:
    typedef ReuseObjectPool<Recyclable<P>> pool_type;
    typedef typename std::conditional<has_intrusive_ptr<Recyclable<P>>::value, boost::intrusive_ptr<Recyclable<P>>, std::shared_ptr<Recyclable<P>>>::type smart_pointer;
    Recyclable() : P() {}
    virtual void suicide()
    {
        pool_type::singleton()->recycle(this);
    }
    static Recyclable<P> *createFromPoolRaw()
    {
        return pool_type::singleton()->create();
    }
    template<typename R = Recyclable<P>>
    static typename std::enable_if <has_intrusive_ptr<R>::value, boost::intrusive_ptr<R>>::type
            createFromPool()
    {
        return createFromPoolRaw();
    }
    template<typename R = Recyclable<P>>
    static typename std::enable_if < !has_intrusive_ptr<R>::value, std::shared_ptr<R >>::type
            createFromPool()
    {
        return std::shared_ptr<R>(createFromPoolRaw(), [ ](R * p)
        {
            p->suicide();
        });
    }
};
}

#endif // REUSEOBJECTPOOL_H
