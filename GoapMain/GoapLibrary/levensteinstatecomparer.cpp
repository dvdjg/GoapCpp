#include "levensteinstatecomparer.h"
#include "basicmath.h"

namespace goap {

LevensteinStateComparer::LevensteinStateComparer() {
}

float LevensteinStateComparer::distance(goap::IState::CPtr stateSrc, goap::IState::CPtr stateDst) const
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
                float dist = valueSrc->levensteinDistance(valueDst);
                percent += dist;
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
