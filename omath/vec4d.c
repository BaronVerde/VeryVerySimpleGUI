
#include "vec4d.h"

inline vec4d* vec4d_set( vec4d* out, const double x, const double y, const double z, const double w ) {
	out->x = x; out->y = y; out->z = z; out->w = w;
	return out;
}
