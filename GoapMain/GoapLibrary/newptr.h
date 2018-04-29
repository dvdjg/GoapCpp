#ifndef NEWPTR_H
#define NEWPTR_H

#include "factory.h"
#include "common/iroot.h"

namespace goap
{

template <typename I, typename P = IRoot>
class NewPtr : public explicit_ptr<I>
{
public:
    template <typename ... Args>
    NewPtr(Args &&... args) : explicit_ptr<I>(Factory<P>::singleton().create<I, Args...>({}, std::forward<Args>(args)...))
    {
    }
};

}

#endif // NEWPTR_H
