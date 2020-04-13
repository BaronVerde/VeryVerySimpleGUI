
#include "vec4f.h"

inline vec4f* vec4f_set( vec4f* out, const float x, const float y, const float z, const float w ) {
	out->x = x; out->y = y; out->z = z; out->w = w;
	return out;
}
