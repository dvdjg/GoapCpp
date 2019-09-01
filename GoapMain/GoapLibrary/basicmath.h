#ifndef BASICMATH_H
#define BASICMATH_H

// #define __float128 long double
#define __STRICT_ANSI__

#include <math.h>
#include "half.h"

namespace basicmath
{

// linear interpolate x in an array
// inline
float interp1(float x, const float a[], int n);
// linear interpolate array a[] -> array b[]
void inter1parray(const float a[], int n, float b[], int m);
//..............................................................................
// parabola through 3 points, -1 < x < 1
float parabola(float x, float f_1, float f0, float f1);
// quadratic interpolate x in an array
float interp2(float x, const float a[], int n);
// quadratic interpolate array a[] -> array b[]
void interp2array(const float a[], int n, float b[], int m);


// linear interpolate x in an array
// inline
float interp1(float x, const half a[], int n);
// linear interpolate array a[] -> array b[]
void inter1parray(const half a[], int n, half b[], int m);
//..............................................................................
// quadratic interpolate x in an array
float interp2(float x, const half a[], int n);
// quadratic interpolate array a[] -> array b[]
void interp2array(const half a[], int n, half b[], int m);




// linear interpolate x in an array
// inline
half interp1(half x, const half a[], int n);
// linear interpolate array a[] -> array b[]
void inter1parrayh(const half a[], int n, half b[], int m);
//..............................................................................
// parabola through 3 points, -1 < x < 1
half parabolah(half x, half f_1, half f0, half f1);
// quadratic interpolate x in an array
half interp2(half x, const half a[], int n);
// quadratic interpolate array a[] -> array b[]
void interp2arrayh(const half a[], int n, half b[], int m);


// http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
std::size_t hash(const half *vec, std::size_t size);
std::size_t hash(const float *vec, std::size_t size);


float cosine_distance(const half *a, const half *b, size_t n);

//////////////////////////////////////////////////////////////////////////
///
///



template<typename OutputType = float, typename InputIterator = half *>
OutputType t_shift_coef(InputIterator begin, size_t n)
{
    OutputType sum = 0;
    int count = 0;
    for (InputIterator it = begin; n--; ++it)
    {
        sum -= *it;
        ++count;
    }
    return sum / count;
}

template<typename OutputType = float, typename InputIterator = half *>
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
        // 1 means are equal, 0 means are different, -1 are opposite
        _distance = (fDen == 0) ? 0 : temp / fDen;
    }

};

template<typename OutputType = float, typename InputIterator = half *>
CosineDistance<OutputType, InputIterator> fnCosineDistance(InputIterator aBegin,
        InputIterator bBegin,
        size_t n)
{
    return CosineDistance<OutputType, InputIterator>(aBegin, bBegin, n);
}

}
#endif // BASICMATH_H
