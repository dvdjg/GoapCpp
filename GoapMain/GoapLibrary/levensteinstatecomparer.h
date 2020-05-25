#ifndef LEVENSTEINSTATECOMPARER_H
#define LEVENSTEINSTATECOMPARER_H

#include "numericstatecomparer.h"

namespace goap {

class LevensteinStateComparer : public NumericStateComparer
{
    IMPLEMENT_REFCOUNTER_PARENT(NumericStateComparer)

public:
    LevensteinStateComparer();

    float distance(IState::CPtr stateSrc, IState::CPtr stateDst) const override;
};

}

#endif // LEVENSTEINSTATECOMPARER_H
