#include "exactstatecomparer.h"
#include "basicmath.h"

namespace goap
{

ExactStateComparer::Ptr ExactStateComparer::singleton()
{
    static Ptr ptrSingleton(new ExactStateComparer());
    return ptrSingleton;
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
    for (IState::index_type i = 0; i < state1->size(); ++i) {
        auto pair1 = state1->at(i);
        auto key = pair1.first;
        auto value1 = pair1.second;
        auto value2 = state2->at(key);
        if (value2 && value1->size() == value2->size()) {
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

