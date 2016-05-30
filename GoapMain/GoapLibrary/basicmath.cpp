// linear, quadratic interpolation in arrays
// from interpol.py denis 2010-07-23 July

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "basicmath.h"

// linear interpolate x in an array
// inline
float interp1( float x, const float a[], int n )
{
    if( x <= 0 )  return a[0];
    if( x >= n - 1 )  return a[n-1];
    int j = int(x);
    return a[j] + (x - j) * (a[j+1] - a[j]);
}

float interp1hf( float x, const half a[], int n )
{
    if( x <= 0 )  return a[0];
    if( x >= n - 1 )  return a[n-1];
    int j = int(x);
    return static_cast<float>(a[j]) + (x - j) * static_cast<float>(a[j+1] - a[j]);
}

// linear interpolate array a[] -> array b[]
void inter1parray( const float a[], int n, float b[], int m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int j = 0; j < m; j ++ ){
        b[j] = interp1( j*step, a, n );
    }
}

void inter1parrayhf( const half a[], int n, half b[], int m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int j = 0; j < m; j ++ ){
        b[j] = interp1hf( j*step, a, n );
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
float interp2( float x, const float a[], int n )
{
    if( x <= .5  ||  x >= n - 1.5 )
        return interp1( x, a, n );
    int j = int( x + .5 );
    float t = 2 * (x - j);  // -1 .. 1
    return parabola( t, (a[j-1] + a[j]) / 2, a[j], (a[j] + a[j+1]) / 2 );
}

float interp2hf( float x, const half a[], int n )
{
    if( x <= .5  ||  x >= n - 1.5 )
        return interp1hf( x, a, n );
    int j = int( x + .5 );
    float t = 2 * (x - j);  // -1 .. 1
    return parabola( t, static_cast<float>(a[j-1] + a[j]) / 2, a[j], static_cast<float>(a[j] + a[j+1]) / 2 );
}

// quadratic interpolate array a[] -> array b[]
void interp2array( const float a[], int n, float b[], int m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int j = 0; j < m; j ++ ){
        b[j] = interp2( j*step, a, n );
    }
}


void interp2arrayhf( const half a[], int n, half b[], int m )
{
    float step = float( n - 1 ) / (m - 1);
    for( int j = 0; j < m; j ++ ){
        b[j] = interp2hf( j*step, a, n );
    }
}


// linear interpolate x in an array
// inline
half interp1( half x, const half a[], int n )
{
    if( x <= 0 )  return a[0];
    if( x >= n - 1 )  return a[n-1];
    int j = int(x);
    return a[j] + (x - j) * (a[j+1] - a[j]);
}

    // linear interpolate array a[] -> array b[]
void inter1parray( const half a[], int n, half b[], int m )
{
    half step = half( n - 1 ) / (m - 1);
    for( int j = 0; j < m; j ++ ){
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
half interp2( half x, const half a[], int n )
{
    if( x <= .5f  ||  x >= n - 1.5f )
        return interp1( x, a, n );
    int j = int( x + .5f );
    half t = 2.f * (x - j);  // -1 .. 1
    return parabolah( t, (a[j-1] + a[j]) / 2.f, a[j], (a[j] + a[j+1]) / 2.f );
}

    // quadratic interpolate array a[] -> array b[]
void interp2array( const half a[], int n, half b[], int m )
{
    half step = half( n - 1 ) / (m - 1);
    for( int j = 0; j < m; j ++ ){
        b[j] = interp2( j*step, a, n );
    }
}

std::size_t hash(const half * vec, std::size_t size)
{
  std::size_t seed = size;
  for(auto it = vec; it != vec+size; ++it) {
    seed ^= it->bits() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}


float cosine_distance(const half *a, const half *b, size_t n)
{
    float temp = 0, fa = 0, fb = 0, va, vb;

    for(size_t i = 0; i < n; ++i)
    {
        va = static_cast<float>(a[i]);
        vb = static_cast<float>(b[i]);
        temp += va * vb;
        fa +=  va * va;
        fb += vb * vb;
    }
    float dist = 1 - (temp / (sqrt(fa) * sqrt(fb)));
    return dist; // 0 means are equal, 2 means are different
}
