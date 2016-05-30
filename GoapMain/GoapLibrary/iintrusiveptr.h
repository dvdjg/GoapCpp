#ifndef IINTRUSIVEPTR_H
#define IINTRUSIVEPTR_H

#include "iroot.h"

//  Relies on unqualified calls to
//
//      void intrusive_ptr_add_ref(T * p);
//      void intrusive_ptr_release(T * p);
//
//          (p != 0)
//
//  The object is responsible for destroying itself.

class IIntrusivePtr : public IRoot
{
public:
};

#endif // IINTRUSIVEPTR_H
