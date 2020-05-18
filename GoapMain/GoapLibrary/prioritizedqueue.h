#ifndef GOAP_PRIORITIZED_H
#define GOAP_PRIORITIZED_H

#include <map>
#include <list>
#include "refcounter.h"
#include "goap/iprioritized.h"
#include "goap/ipath.h"

namespace goap
{

class PrioritizedQueue : public virtual IPrioritized
{
    IMPLEMENT_REFCOUNTER()

protected:
    typedef std::list<IPath::Ptr> lst_path_type;
    typedef std::map<std::int64_t, lst_path_type> map_lst_path_type;
    map_lst_path_type _queues;
    std::int64_t _min = LLONG_MAX;
    lst_path_type _lazyArray;

public:
    typedef explicit_ptr<PrioritizedQueue> Ptr;
    typedef explicit_ptr<const PrioritizedQueue> CPtr;

    bool empty() override;

    std::int64_t size() const override;

    /**
     * The path is inserted in the processing list.
     * If its cost is lower than current level it will be executed the next.
     */
    void push(IPath::Ptr path) override;

    /**
     * The path will be processed after all the paths of the current Cost.
     */
    void pushLazy(IPath::Ptr path) override;

    /**
     * Retrieves the next path stored with the lower cost.
     */
    IPath::Ptr pop() override;

    void clear() override;

protected:
    virtual IPath::Ptr pop_queue(lst_path_type &queue);
    std::int64_t currentPriority();

    void useLazyArray();

    // IStringPrintable interface
public:
    string toDebugString() const override;
    string toString() const override;
};

}

#endif // GOAP_PRIORITIZED_H
