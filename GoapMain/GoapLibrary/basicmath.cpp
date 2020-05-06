// linear, quadratic interpolation in arrays
// from interpol.py denis 2010-07-23 July

#include <stdio.h>
#include <stdlib.h>
#include "basicmath.h"

namespace basicmath
{
float floatSimilarity(float x, float y)
{
    float ret = 1;
    // 0 < are similar < are different < 1
    float diff = std::abs(x - y);
    if (diff > GOAP_FLT_EPSILON * std::abs(x + y) * 2 && diff > GOAP_FLT_MIN) {
        float ax = std::abs(x);
        float ay = std::abs(y);
        ret = diff / std::max(ax, ay);
    }
    return ret;
}

bool floatEqual(float x, float y)
{
    float diff = std::abs(x - y);
    return diff <= GOAP_FLT_EPSILON * std::abs(x + y) * 2 || diff < GOAP_FLT_MIN;
}

// linear interpolate x in an array
// inline
float interp1( float x, const float a[], int_type n )
{
    if( x <= 0 )  return a[0];
    if( x >= n - 1 )  return a[n-1];
    int_type j = int_type(x);
    return a[j] + (x - j) * (a[j+1] - a[j]);
}

float interp1( float x, const half a[], int_type n )
{
    if( x <= 0 )  return a[0];
    if( x >= n - 1 )  return a[n-1];
    int_type j = int_type(x);
    return static_cast<float>(a[j]) + (x - j) * static_cast<float>(a[j+1] - a[j]);
}

// linear interpolate array a[] -> array b[]
void inter1parray( const float a[], int_type n, float b[], int_type m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int_type j = 0; j < m; j ++ ){
        b[j] = interp1( j*step, a, n );
    }
}

void inter1parray( const half a[], int_type n, half b[], int_type m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int_type j = 0; j < m; j ++ ){
        b[j] = interp1( j*step, a, n );
    }
}

//..............................................................................
    // parabola through 3 points, -1 < x < 1
float parabola( float x, float f_1, float f0, float f1 )
{
    if( x <= -1 )  return f_1;
    if( x >= 1 )  return f1;
    float l = f0 - x * (f_1 - f0);
    float r = f0 + x * (f1 - f0);
    return (l + r + x * (r - l)) / 2;
}

// quadratic interpolate x in an array
float interp2( float x, const float a[], int_type n )
{
    if( x <= .5f  ||  x >= n - 1.5f )
        return interp1( x, a, n );
    int_type j = int_type( x + .5f );
    float t = 2.f * (x - j);  // -1 .. 1
    return parabola( t, (a[j-1] + a[j]) / 2, a[j], (a[j] + a[j+1]) / 2 );
}

float interp2( float x, const half a[], int_type n )
{
    if( x <= .5f  ||  x >= n - 1.5f )
        return interp1( x, a, n );
    int_type j = int_type( x + .5f );
    float t = 2 * (x - j);  // -1 .. 1
    return parabola( t, static_cast<float>(a[j-1] + a[j]) / 2, a[j], static_cast<float>(a[j] + a[j+1]) / 2 );
}

// quadratic interpolate array a[] -> array b[]
void interp2array( const float a[], int_type n, float b[], int_type m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int_type j = 0; j < m; j ++ ){
        b[j] = interp2( j*step, a, n );
    }
}


void interp2array( const half a[], int_type n, half b[], int_type m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int_type j = 0; j < m; j ++ ){
        b[j] = interp2( j*step, a, n );
    }
}


// linear interpolate x in an array
// inline
half interp1( half x, const half a[], int_type n )
{
    if( x <= 0 )  return a[0];
    if( x >= n - 1 )  return a[n-1];
    int_type j = int_type(x);
    return a[j] + (x - j) * (a[j+1] - a[j]);
}

    // linear interpolate array a[] -> array b[]
void inter1parrayh( const half a[], int_type n, half b[], int_type m )
{
    half step = half( n - 1 ) / (m - 1);
    for( int_type j = 0; j < m; j ++ ){
        b[j] = interp1( j*step, a, n );
    }
}

//..............................................................................
    // parabola through 3 points, -1 < x < 1
half parabolah( half x, half f_1, half f0, half f1 )
{
    if( x <= -1 )  return f_1;
    if( x >= 1 )  return f1;
    half l = f0 - x * (f_1 - f0);
    half r = f0 + x * (f1 - f0);
    return (l + r + x * (r - l)) / 2;
}

    // quadratic interpolate x in an array
half interp2( half x, const half a[], int_type n )
{
    if( x <= .5f  ||  x >= n - 1.5f )
        return interp1( x, a, n );
    int_type j = int_type( x + .5f );
    half t = 2.f * (x - j);  // -1 .. 1
    return parabolah( t, (a[j-1] + a[j]) / 2.f, a[j], (a[j] + a[j+1]) / 2.f );
}

    // quadratic interpolate array a[] -> array b[]
void interp2arrayh( const half a[], int_type n, half b[], int_type m )
{
    half step = half( n - 1 ) / (m - 1);
    for( int_type j = 0; j < m; j ++ ){
        b[j] = interp2( j*step, a, n );
    }
}

std::size_t hash(const unsigned short * vec, std::size_t size)
{
  std::size_t seed = size;
  for(auto it = vec; it != vec+size; ++it) {
    seed ^= *it + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

std::size_t hash(const half * vec, std::size_t size)
{
  const unsigned short * vecs = reinterpret_cast<const unsigned short *>(vec);
  return hash(vecs, size);
}

std::size_t hash(const float * vec, std::size_t size)
{
  const unsigned short * vecs = reinterpret_cast<const unsigned short *>(vec);
  return hash(vecs, size*2);
}

template<typename OutputType = float, typename InputIterator = half*>
OutputType t_cosine_distance(InputIterator aBegin,
                             InputIterator aEnd,
                             InputIterator bBegin)
{
    OutputType temp = 0, fa = 0, fb = 0;

    for(InputIterator aIt = aBegin, bIt = bBegin; aIt != aEnd; ++aIt, ++bIt)
    {
        OutputType va = *aIt;
        OutputType vb = *bIt;
        temp += va * vb;
        fa +=  va * va;
        fb += vb * vb;
    }
    OutputType dist = 1 - (temp / (sqrt(fa) * sqrt(fb)));
    return dist; // 0 means are equal, 2 means are different
}

template<typename OutputType = float, typename InputIterator = half*>
OutputType t_cosine_distance(InputIterator aBegin,
                             InputIterator aEnd,
                             InputIterator bBegin,
                             OutputType aOffset,
                             OutputType bOffset
                             )
{
    OutputType temp = 0, fa = 0, fb = 0;

    for(InputIterator aIt = aBegin, bIt = bBegin; aIt != aEnd; ++aIt, ++bIt)
    {
        OutputType va = *aIt + aOffset;
        OutputType vb = *bIt + bOffset;
        temp += va * vb;
        fa +=  va * va;
        fb += vb * vb;
    }
    OutputType dist = 1 - (temp / (sqrt(fa) * sqrt(fb)));
    return dist; // 0 means are equal, 2 means are different
}


float cosine_distance(const half *a, const half *b, size_t n)
{
    float dist = t_cosine_distance<float>(a, b, a+n);
    return dist; // 1 means are same direction, 0 means perpendicular, -1 means are completely opposite direction
}

}
