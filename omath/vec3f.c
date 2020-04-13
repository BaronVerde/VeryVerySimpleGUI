
#include "vec3f.h"

inline vec3f* vec3f_set( vec3f* out, const float x, const float y, const float z ) {
	out->x = x; out->y = y; out->z = z;
	return out;
}

inline vec3f* vec3f_set_from( vec3f* out, const vec3f* const other ) {
	out->x = other->x; out->y = other->y; ; out->z = other->z;
	return out;
}

inline vec3f* vec3f_sub( vec3f* out, const vec3f* const a, const vec3f* const b ) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	return out;
}

inline float vec3f_magnitude( const vec3f* const a ) {
	return sqrtf( a->x * a->x + a->y * a->y + a->z * a->z );
}

inline float vec3f_dot( const vec3f* const a, const vec3f* const b ) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

inline vec3f* vec3f_normalize( vec3f* out, const vec3f* const a ) {
	float len = vec3f_magnitude( a );
#if ENABLE_SAFETY_CHECKS
	assert( !fcmp( len, 0.0f ) );
#endif
	float k = 1.0f / len;
	out->x = a->x * k;
	out->y = a->y * k;
	out->z = a->z * k;
	return out;
}

inline vec3f* vec3f_cross( vec3f* out, const vec3f* const a, const vec3f* const b ) {
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
	return out;
}
