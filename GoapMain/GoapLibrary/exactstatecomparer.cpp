#include <algorithm>
#include "exactstatecomparer.h"
#include "basicmath.h"
#include "newptr.h"

namespace goap
{

ExactStateComparer::Ptr ExactStateComparer::singleton()
{
    static std::string discr(STR_GOAP_EXACTSTATECOMPARER_SINGLETON);
    return NewPtr<ExactStateComparer>(discr);
}

float ExactStateComparer::distancePure(IState::CPtr state1, IState::CPtr state2) const
{
    float percent = 0;
    IState::index_type s2Count = 0;
    auto itState = state2->iterator();
    while (itState->hasNext()) {
        auto pair = itState->next();
        auto key = pair.first;
        auto value2 = pair.second;
        if (value2) {
            auto value1 = state1->at(key);
            if (!value1 || *value1 != *value2) {
                percent += 1;
            }
        }
        ++s2Count;
    }
    auto ret = (s2Count == 0) ? 1.f : percent / s2Count;
    return ret;
}

/**
 * Gives an idea of how far are two states one to another. If Distance == 0, both are equal
 * A percentage exact state members count is used to compute de similarities.
 * Both states can be of any type. The != operator is used to find similarities
 */
float ExactStateComparer::distance(IState::CPtr state1, IState::CPtr state2) const
{
    float percent = 0;
    float countDifferent = 0;
    IState::index_type s2Count = 0;
    IState::index_type sameKeyCount = 0; // Counts the number of keys that state1 has like state2
    auto itState = state2->iterator();
    while (itState->hasNext()) {
        auto pair = itState->next();
        auto key = pair.first;
        auto value2 = pair.second;
        if (value2) {
            auto value1 = state1->at(key);
            if (value1) {
                sameKeyCount++;
                if (*value1 != *value2) {
                    percent += 1;
                }
            }
        }
        ++s2Count;
    }

    countDifferent = s2Count + state1->size() - sameKeyCount - sameKeyCount;
    auto ret = (percent + countDifferent) / (sameKeyCount + countDifferent);
    return ret;
}

}

