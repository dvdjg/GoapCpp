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
bool NumericStateComparer::enough(IState::CPtr stateSrc, IState::CPtr stateDst) const
{
    for (IState::index_type i = 0; i < stateDst->size(); ++i) {
        auto pair1 = stateDst->at(i);
        auto key = pair1.first;
        auto value1 = pair1.second;
        auto value2 = stateSrc->at(key);
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
float NumericStateComparer::distance(IState::CPtr stateSrc, IState::CPtr stateDst) const
{
    float percent = 0;
    float countDifferent = 0;
    int s2Count = 0;
    int sameKeyCount = 0; // Counts the number of keys that stateSrc has like stateDst
    for (IState::index_type i = 0; i < stateDst->size(); ++i) {
        auto pairDst = stateDst->at(i);
        auto key = pairDst.first;
        auto valueDst = pairDst.second;
        auto valueSrc = stateSrc->at(key);
        if (valueSrc && valueSrc->size() > 0) {
            if (valueDst->size() == 1 && valueSrc->size() == 1) {
                sameKeyCount++;
                // Si son números únicos, hacer una comparación sencilla
                float s1k = valueSrc->at(0);
                float s2k = valueDst->at(0);
                // 0 < are similar < are different < 1
                percent += basicmath::floatSimilarity(s1k, s2k);
            } else if (valueDst->size() >= 1 && valueSrc->size() >= 1) {
                sameKeyCount++;
                float thisModule, othersModule;
                float cosDist = valueSrc->cosineDistance(valueDst, &thisModule, &othersModule);
                float similiarity = basicmath::floatSimilarity(thisModule, othersModule);
                percent += std::max(0.f, cosDist * similiarity);
            }
            //LOG(DEBUG) << "Compared key=" << *key << ": ValueDst=" << *valueDst << "; ValueSrc=" << *valueSrc << ". AccPercent=" << percent;
        }
        ++s2Count;
    }
    countDifferent = s2Count - sameKeyCount;
    float dist = (percent + countDifferent) / s2Count;
    LOG(DEBUG) << "Distance=" << dist << "\n  from " << *stateSrc << "\n    to " << *stateDst;
    //LOG(DEBUG) << "Distance=" << dist;
    return dist;
}

}

