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



}
#endif // BASICMATH_H
