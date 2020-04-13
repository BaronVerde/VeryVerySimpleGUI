
#pragma once

//#define ENABLE_SAFETY_CHECKS 1
#define _XOPEN_SOURCE	// for drand48 to be available
#define _GNU_SOURCE		// sincos() in mat4d.c
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

// Global consts. I don't like the math.h wording. Mind definitions in shaders !
#define SQRT2 1.4142135623730950488
#define SQRT3 1.7320508075688772935
#define PI 3.1415926535897932385
#define ONE_OVER_PI ( 1.0 / PI )
#define ONE_OVER_TWO_PI ( 1.0 / ( 2.0 * PI ) )
#define PI_OVER_TWO ( PI * 0.5 )
#define PI_OVER_THREE ( PI / 3.0 )
#define PI_OVER_FOUR ( PI / 4.0 )
#define PI_OVER_SIX ( PI / 6.0 )
#define THREE_PI_OVER_TWO ( 1.5 * PI )
#define TWO_PI ( 2.0 * PI )

extern void init();

// returns v0 when t == 0 and v1 when t == 1
extern double lerp( const double v0, const double v1, const double t );

// Interpolates between n1 and n2 with given pre-n1 value n0 and post-n1 value n3
// If a is 0, function returns n1, if a is 1.0, function returns n2
// After libnoise ! @fixme Default value for a should be 0.5 !
extern double cubic_interpolate( const double n0, const double n1, const double n2, const double n3, const double a /*= 0.5*/ );

extern bool is_pow2( const long int val );

extern double clamp( const double number, const double minimum, const double maximum );

// Compares two float numbers combining absolute and relative tolerance.
// http://www.realtimecollisiondetection.net/pubs/doubleolerances/
extern bool fcmp_r( const double a, const double b, const double maxDiff, const double maxRelDiff );

// Floating point absolute comparison
extern bool fcmp( const double x, const double y );

// radians
extern double radians( const double degrees );

// degrees
extern double degrees( const double radians );

extern int signum( const double val );

extern void double_to_two_floats( const double d, float* high, float* low );
