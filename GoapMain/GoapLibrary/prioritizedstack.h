#ifndef PRIORITIZEDSTACK_H
#define PRIORITIZEDSTACK_H

#include "prioritizedqueue.h"

namespace goap
{

class PrioritizedStack : public PrioritizedQueue
{
    IPath::CPtr pop_queue(lst_path_type &queue) override {
        if (queue.empty()) {
            throw new std::runtime_error("[PrioritizedQueue] stack pop error");
        }
        IPath::CPtr value = queue.back();
        queue.pop_back();
        return value;
    }
};

}

#endif // PRIORITIZEDSTACK_H
