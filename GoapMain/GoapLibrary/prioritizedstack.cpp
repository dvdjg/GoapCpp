#include <stdexcept>
#include <sstream>
#include "prioritizedstack.h"

namespace goap
{
IPath::Ptr PrioritizedStack::pop_queue(PrioritizedQueue::lst_path_type &queue) {
    if (queue.empty()) {
        throw std::runtime_error("[PrioritizedQueue] stack pop error");
    }
    IPath::Ptr value = queue.back();
    queue.pop_back();
    return value;
}

string PrioritizedStack::toDebugString() const
{
    stringstream ss;
    ss << "PrioritizedStack: { min:" << _min << ", _queues_size:" << _queues.size() << ", lazyArray_size:" << _lazyArray.size() << "}" << endl;
    return ss.str();
}

}


