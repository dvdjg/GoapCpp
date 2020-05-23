#ifndef GOAP_COSINEDISTANCE_H
#define GOAP_COSINEDISTANCE_H

namespace basicmath
{
typedef unsigned long long size_t;

template<typename OutputType = float, typename InputIterator = float *>
OutputType t_shift_coef(InputIterator begin, size_t n)
{
    OutputType sum = 0;
    int count = 0;
    for (InputIterator it = begin; n--; ++it)
    {
        sum -= *it;
        ++count;
    }
    return (count == 0) ? 0 : sum / count;
}

template<typename OutputType = float, typename InputIterator = float *>
class CosineDistance
{
    OutputType _distance = 0;
    OutputType _aModule = 0;
    OutputType _bModule = 0;
    OutputType _aShift = 0;
    OutputType _bShift = 0;

public:
    CosineDistance(InputIterator aBegin,
                   InputIterator bBegin,
                   size_t n)
    {
        _aShift = t_shift_coef(aBegin, n);
        _bShift = t_shift_coef(bBegin, n);
        offsetCosineDistance(aBegin, bBegin, n, _aShift, _bShift);
    }

    OutputType similarity() const
    {
        return 1 - _distance;    // 0 means equal
    }
    OutputType distance() const
    {
        return _distance;
    }

    /// Gives an idea of the magnitude of the vector
    OutputType aModule() const
    {
        return _aModule;
    }
    OutputType bModule() const
    {
        return _bModule;
    }

    /// Offset to add to all components to be centered
    OutputType aShift() const
    {
        return _aShift;
    }
    OutputType bShift() const
    {
        return _bShift;
    }

protected:
    void offsetCosineDistance(InputIterator aBegin,
                              InputIterator bBegin,
                              size_t n,
                              OutputType aOffset,
                              OutputType bOffset)
    {
        OutputType temp = 0, fa = 0, fb = 0;

        for (InputIterator aIt = aBegin, bIt = bBegin; n--; ++aIt, ++bIt)
        {
            OutputType va = *aIt + aOffset;
            OutputType vb = *bIt + bOffset;
            temp += va * vb;
            fa +=  va * va;
            fb += vb * vb;
        }
        _aModule = sqrt(fa);
        _bModule = sqrt(fb);
        OutputType fDen = _aModule * _bModule;
        // 1 means are same direction, 0 means are different, -1 are opposite
        _distance = (fDen == 0) ? 0 : temp / fDen;
    }

};

template<typename OutputType = float, typename InputIterator = float *>
CosineDistance<OutputType, InputIterator> fnCosineDistance(InputIterator aBegin,
        InputIterator bBegin,
        size_t n)
{
    return CosineDistance<OutputType, InputIterator>(aBegin, bBegin, n);
}

}

#endif // GOAP_COSINEDISTANCE_H
