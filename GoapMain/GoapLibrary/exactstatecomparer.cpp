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

float ExactStateComparer::distancePure(IState::CPtr state1, IState::CPtr state2) const
{
    float percent = 0;
    int s2Count = 0;
    for (IState::index_type i = 0; i < state2->size(); ++i) {
        auto pair2 = state2->at(i);
        auto key = pair2.first;
        auto value2 = pair2.second;
        auto value1 = state1->at(key);
        if (*value1 != *value2) {
            percent += 1;
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
    int s2Count = 0;
    int sameKeyCount = 0; // Counts the number of keys that state1 has like state2
    for (IState::index_type i = 0; i < state2->size(); ++i) {
        auto pair2 = state2->at(i);
        auto key = pair2.first;
        auto value2 = pair2.second;
        auto value1 = state1->at(key);
        if (value1 && value2->size() == value1->size()) {
            sameKeyCount++;
            if (*value1 != *value2) {
                percent += 1;
            }
        }
        ++s2Count;
    }
    //auto len = std::max(state1->size(), state2->size());

    countDifferent = s2Count + state1->size() - sameKeyCount - sameKeyCount;
    auto ret = (percent + countDifferent) / (sameKeyCount + countDifferent);
    return ret;
}

}

