#ifndef NEWPTR_H
#define NEWPTR_H

#include "factory.h"
#include "common/iroot.h"
#include "explicit_ptr.h"

namespace goap
{

template <typename I>
class NewPtr : public explicit_ptr<I>
{
public:
    template <typename ... Args>
    NewPtr(Args &&... args) : explicit_ptr<I>(Factory<IRoot>::singleton().create<I, Args...>({}, std::forward<Args>(args)...))
    {
    }
};

}

#endif // NEWPTR_H
