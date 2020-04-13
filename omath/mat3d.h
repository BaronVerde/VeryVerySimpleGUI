
#pragma once

#include "vec3d.h"

typedef struct {
	double data[9];
} mat3d;

inline mat3d* mat3d_set( mat3d *out,
		const double _11, const double _12, const double _13,
		const double _21, const double _22, const double _23,
		const double _31, const double _32, const double _33 ) {
	out->data[0] = _11; out->data[1] = _12; out->data[2] = _13;
	out->data[3] = _21; out->data[4] = _22; out->data[5] = _23;
	out->data[6] = _31; out->data[7] = _32; out->data[8] = _33;
	return out;
}

inline void mat3d_identity( mat3d* out ) {
	*out = (mat3d) { .data = { 1, 0, 0,
							   0, 1, 0,
							   0, 0, 1 } };
}
