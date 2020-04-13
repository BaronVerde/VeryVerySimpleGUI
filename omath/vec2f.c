
#include "vec2f.h"

inline vec2f* vec2f_set( vec2f* out, const double x, const double y ) {
	out->x = x; out->y = y;
	return out;
}

inline vec2f* vec3d_set_from( vec2f* out, const vec2f* const other ) {
	out->x = other->x; out->y = other->y;
	return out;
}
