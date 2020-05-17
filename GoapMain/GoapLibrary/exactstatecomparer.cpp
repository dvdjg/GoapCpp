#include <algorithm>
#include "exactstatecomparer.h"
#include "basicmath.h"
#include "newptr.h"

namespace goap
{

ExactStateComparer::Ptr ExactStateComparer::singleton()
{
    static std::string discr(STR_GOAP_EXACTSTATEMETER_SINGLETON);
    return NewPtr<ExactStateComparer>(discr);
}

/**
 * Returns the measure from the input state to the goal state.
 * It will return lower numbers when it deems that the goal is close so the planner can direct the search.
 */
float ExactStateComparer::distance(IState::CPtr state1, IState::CPtr state2) const
{
    float percent = 0;
    float countDifferent = 0;
    int s2Count = 0;
    int sameKeyCount = 0; // Counts the number of keys that state1 has like state2
    for (IState::index_type i = 0; i < state2->size(); ++i) {
        auto pair2 = state2->at(i);
        auto key = pair2.first;
        auto value2 = pair2.second;
        auto value1 = state1->at(key);
        if (value1 && value2->size() == value1->size()) {
            sameKeyCount++;
            if (value1 != value2) {
                percent += 1;
            }
        }
        ++s2Count;
    }
    auto len = std::max(state1->size(), state2->size());

    countDifferent = s2Count + len - sameKeyCount - sameKeyCount;
    auto ret = (percent + countDifferent) / (sameKeyCount + countDifferent);
    return ret;
}

}

