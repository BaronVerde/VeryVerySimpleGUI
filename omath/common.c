
#include "common.h"
#include <limits.h>
#include <float.h>	// epsilon
#include <time.h>	// init srand()

inline void init() {
	srand48( time(0) );
}

inline double lerp( const double v0, const double v1, const double t ) {
  return( ( 1 - t ) * v0 + t * v1 );
}

inline double cubic_interpolate( const double n0, const double n1, const double n2, const double n3, const double a ) {
	const double p = ( n3 - n2 ) - ( n0 - n1 );
	const double q = ( n0 - n1 ) - p;
	const double r = n2 - n0;
	return p * a * a * a + q * a * a + r * a + n1;
}

inline bool is_pow2( const long int val ) {
	if( val < 1 )
		return false;
	return ( val & ( val - 1 ) ) == 0 ? true : false;
}

inline double clamp( const double number, const double minimum, const double maximum ) {
	return ( number < minimum ) ? minimum : ( number > maximum ) ? maximum : number;
}

inline bool fcmp_r( const double a, const double b, const double maxDiff, const double maxRelDiff ) {
	const double diff = fabs( a - b );
	if( diff <= maxDiff )
		return true;
	const double aa = fabs(a);
	const double ab = fabs(b);
	const double largest = ( ab > aa ) ? ab : aa;
	if( diff <= largest * maxRelDiff )
		return true;
	return false;
}

inline double radians( const double degrees ) {
	return degrees * 0.01745329251994329576923690768489;
}

inline double degrees( const double radians ) {
	return radians * 57.295779513082320876798154814105;
}

inline bool fcmp( const double x, const double y ) {
	return fabs( x - y ) <= FLT_EPSILON * fmax( 1.0, fmax( fabs( x ), fabs( y ) ) );
}

inline void double_to_two_floats( const double d, float* high, float* low ) {
	*high = (float)d;
	*low = (float)( d - (double)(*high) );
}
