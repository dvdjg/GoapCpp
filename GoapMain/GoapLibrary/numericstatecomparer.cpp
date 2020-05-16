#include <algorithm>
#include "numericstatecomparer.h"
#include "basicmath.h"
#include "newptr.h"
#include "goaplibrary.h"

namespace goap
{

NumericStateComparer::Ptr NumericStateComparer::singleton()
{
    return NewPtr<NumericStateComparer>(NUMERICSTATECOMPARER_SINGLETON);
}

NumericStateComparer::NumericStateComparer() {
}

/**
 * Returns true when the state has reach the goal, so the input 'state' satisfies the goal state.
 */
bool NumericStateComparer::enough(IState::CPtr state1, IState::CPtr state2) const
{
    for (IState::index_type i = 0; i < state1->size(); ++i) {
        auto pair1 = state1->at(i);
        auto key = pair1.first;
        auto value1 = pair1.second;
        auto value2 = state2->at(key);
        if (value1 != value2) {
            return false;
        }
    }
    return true;
}

/**
 * Returns the measure from the input state to the goal state.
 * It will return lower numbers when it deems that the goal is close so the planner can direct the search.
 */
float NumericStateComparer::distance(IState::CPtr state1, IState::CPtr state2) const
{
    float percent = 0;
    float countDifferent = 0;
    int s2Count = 0;
    int sameKeyCount = 0; // Counts the number of keys that state1 has like state2
    LOG(DEBUG) << "Distance\n from " << *state1 << "\n to " << *state2;
    for (IState::index_type i = 0; i < state1->size(); ++i) {
        auto pair1 = state1->at(i);
        auto key = pair1.first;
        auto value1 = pair1.second;
        auto value2 = state2->at(key);
        LOG(DEBUG) << "Compare key=" << *key << ": Value1=" << *value1 << "; Value2=" << *value2;
        if (value2 && value2->size() > 0) {
            if (value1->size() == 1 && value2->size() == 1) {
                sameKeyCount++;
                // Si son números únicos, hacer una comparación sencilla
                float s1k = value1->at(0);
                float s2k = value2->at(0);
                // 0 < are similar < are different < 1
                percent += basicmath::floatSimilarity(s1k, s2k);
            } else if (value1->size() >= 1 && value2->size() >= 1) {
                sameKeyCount++;
                float thisModule, othersModule;
                float cosDist = value1->cosineDistance(value2, &thisModule, &othersModule);
                float similiarity = basicmath::floatSimilarity(thisModule, othersModule);
                percent += std::max(0.f, cosDist * similiarity);
            }
        }
        ++s2Count;
    }
    countDifferent = s2Count - sameKeyCount;
    float dist = (percent + countDifferent) / s2Count;
    LOG(DEBUG) << "Distance=" << dist;
    return dist;
}

}

