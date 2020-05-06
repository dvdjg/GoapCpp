#ifndef GOAP_ICLONABLE_H
#define GOAP_ICLONABLE_H

#include "common/irefcounter.h"
#include "explicit_ptr.h"

namespace goap
{

class IClonable : public virtual IRefCounter
{
public:
    typedef explicit_ptr<IClonable> Ptr;
    typedef explicit_ptr<const IClonable> CPtr;

    virtual Ptr clone() const = 0;
};

}

#endif // GOAP_ICLONABLE_H
