#include <stdexcept>
#include "prioritizedqueue.h"

namespace goap
{

bool PrioritizedQueue::empty() {
    if (!_queues.empty()) {
        return false;
    }
    if (!_lazyArray.empty()) {
        _min = LLONG_MAX;
        useLazyArray();
        return false;
    }
    return true;
}

int64_t PrioritizedQueue::size() const {
    return _queues.size();
}

void PrioritizedQueue::push(IPath::Ptr path) {
    std::int64_t icost = 10000000.0 * path->cost();
    if (_queues.find(icost) == _queues.end()) {
        if (_min > icost) {
            _min = icost;
        }
    }
    auto &queue = _queues[icost];
    queue.push_back(path);
}

void PrioritizedQueue::pushLazy(IPath::Ptr path) {
    _lazyArray.push_back(path);
}

IPath::Ptr PrioritizedQueue::pop() {
    if (empty()) {
        return {};
    }
    // Takes the minimum cost queue
    std::int64_t min = _min;
    auto &queue = _queues[min];
    IPath::Ptr value = pop_queue(queue);
    if (queue.empty()) {
        // Use the lazy array when current has finished
        useLazyArray();
        if (queue.empty()) {
            // If after reinserting elements this buffer continues empty, then delete it
            _queues.erase(min);
            if (min == _min) {
                // The min value is invalid, so we need to recompute it
                _min = LLONG_MAX;
                for (auto &it : _queues) {
                    if (it.first < _min) {
                        _min = it.first;
                    }
                }
            }
        }
    }
    return value;
}

void PrioritizedQueue::clear() {
    _queues.clear();
    _lazyArray.clear();
    _min = LLONG_MAX;
}

IPath::Ptr PrioritizedQueue::pop_queue(PrioritizedQueue::lst_path_type &queue) {
    if (queue.empty()) {
        throw std::runtime_error("[PrioritizedQueue] queue pop error");
    }
    IPath::Ptr value = queue.front();
    queue.pop_front();
    return value;
}

int64_t PrioritizedQueue::currentPriority() {
    return _min;
}

void PrioritizedQueue::useLazyArray() {
    for (auto path = _lazyArray.front(); !_lazyArray.empty(); _lazyArray.pop_front()) {
        // reinsert all retained data to the processing buffers
        push(path);
    }
}

}
