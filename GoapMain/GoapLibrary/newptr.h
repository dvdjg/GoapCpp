#ifndef NEWPTR_H
#define NEWPTR_H

#include "factory.h"
#include "common/iroot.h"

namespace goap
{

template <typename I, typename P = IRoot, typename Key = std::string>
class NewPtr : public explicit_ptr<I>
{
public:
    template <typename ... Args>
    NewPtr(const Key & discr, Args &&... args) : explicit_ptr<I>(Factory<P, Key>::singleton().template create<I, Args...>(discr, std::forward<Args>(args)...))
    {
    }
    template <size_t N, typename ... Args>
    NewPtr(const Key & discr, const char (&sz)[N], Args &&... args) : explicit_ptr<I>(Factory<P, Key>::singleton().template create<I, const char *, Args...>(discr, std::forward<const char *>(sz), std::forward<Args>(args)...))
    {
    }
    template <typename T, typename ... Args>
    NewPtr(std::initializer_list<T> lst, Args &&... args) : explicit_ptr<I>(Factory<P, Key>::singleton().template create<I, std::initializer_list<T>, Args...>(Key(), std::forward<std::initializer_list<T>>(lst), std::forward<Args>(args)...))
    {
    }
    NewPtr() : explicit_ptr<I>(Factory<P, Key>::singleton().template create<I>({}))
    {
    }
    //template<typename T>
    //NewPtr(const explicit_ptr<T> & other) : explicit_ptr<I>(other)
    //{
    //}
    NewPtr(const explicit_ptr<I> & other) : explicit_ptr<I>(other)
    {
    }

    explicit_ptr<I> getPtr() {
        return *this;
    }
    explicit_ptr<const I> getCPtr() {
        return explicit_ptr<I>(*this);
    }
};



}

#endif // NEWPTR_H
