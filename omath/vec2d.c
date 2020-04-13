
#include "vec2d.h"

inline vec2d* vec2d_set( vec2d* out, const double x, const double y ) {
	out->x = x; out->y = y;
	return out;
}

inline vec2d* vec3d_set_from( vec2d* out, const vec2d* const other ) {
	out->x = other->x; out->y = other->y;
	return out;
}
