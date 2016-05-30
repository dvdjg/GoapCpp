#ifndef BASICMATH_H
#define BASICMATH_H

#include "../half/half.h"

// linear interpolate x in an array
// inline
float interp1( float x, const float a[], int n );
// linear interpolate array a[] -> array b[]
void inter1parray( const float a[], int n, float b[], int m );
//..............................................................................
// parabola through 3 points, -1 < x < 1
float parabola( float x, float f_1, float f0, float f1 );
// quadratic interpolate x in an array
float interp2( float x, const float a[], int n );
// quadratic interpolate array a[] -> array b[]
void interp2array( const float a[], int n, float b[], int m );


// linear interpolate x in an array
// inline
float interp1hf( float x, const half a[], int n );
// linear interpolate array a[] -> array b[]
void inter1parrayhf( const half a[], int n, half b[], int m );
//..............................................................................
// quadratic interpolate x in an array
float interp2hf( float x, const half a[], int n );
// quadratic interpolate array a[] -> array b[]
void interp2arrayhf( const half a[], int n, half b[], int m );




// linear interpolate x in an array
// inline
half interp1( half x, const half a[], int n );
// linear interpolate array a[] -> array b[]
void inter1parray( const half a[], int n, half b[], int m );
//..............................................................................
// parabola through 3 points, -1 < x < 1
half parabolah( half x, half f_1, half f0, half f1 );
// quadratic interpolate x in an array
half interp2(half x, const half a[], int n );
// quadratic interpolate array a[] -> array b[]
void interp2array( const half a[], int n, half b[], int m );


// http://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
std::size_t hash(const half * vec, std::size_t size);

#endif // BASICMATH_H
