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
    auto itState = stateDst->iterator();
    while (itState->hasNext()) {
        auto pair = itState->next();
        auto key = pair.first;
        IStateValue::Ptr value1 = pair.second;
        if (value1) {
            auto value2 = stateSrc->at(key);
            if (!value2 || *value1 != *value2) {
                return false;
            }
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
    IState::index_type s2Count = 0;
    IState::index_type sameKeyCount = 0; // Counts the number of keys that stateSrc has like stateDst

    auto itState = stateDst->iterator();
    while (itState->hasNext()) {
        auto pair = itState->next();
        auto key = pair.first;
        auto valueDst = pair.second;
        if (valueDst) {
            auto valueSrc = stateSrc->at(key);
            if (valueSrc) {
                sameKeyCount++;
                if (valueDst->size() == 1 && valueSrc->size() == 1) {
                    // Si son números únicos, hacer una comparación sencilla
                    float s1k = valueSrc->at(0);
                    float s2k = valueDst->at(0);
                    // 0 < are similar < are different < 1
                    percent += basicmath::floatSimilarity(s1k, s2k);
                } else if (valueDst->size() >= 1 && valueSrc->size() >= 1) {
                    float thisModule, othersModule;
                    float cosDist = valueSrc->cosineDistance(valueDst, &thisModule, &othersModule);
                    float similiarity = 1.f - basicmath::floatSimilarity(thisModule, othersModule);
                    float percInc = std::max(0.f, 1.f - cosDist * similiarity);
                    percent += percInc;
                } else {
                    percent += 1.f;
                }
                //LOG(DEBUG) << "Compared key=" << *key << ": ValueDst=" << *valueDst << "; ValueSrc=" << *valueSrc << ". AccPercent=" << percent;
            }
        }
        ++s2Count;
    }
    IState::index_type countDifferent = s2Count - sameKeyCount;
    float dist = (percent + countDifferent) / s2Count;
    //LOG(DEBUG) << "NumericDistance=" << dist << "\n  from " << *stateSrc << "\n    to " << *stateDst;
    return dist;
}

}

