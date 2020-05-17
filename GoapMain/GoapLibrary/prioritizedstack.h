#ifndef PRIORITIZEDSTACK_H
#define PRIORITIZEDSTACK_H

#include "prioritizedqueue.h"

namespace goap
{

class PrioritizedStack : public PrioritizedQueue
{
    IPath::Ptr pop_queue(lst_path_type &queue) override;
    string toDebugString() const override;
};

}

#endif // PRIORITIZEDSTACK_H
