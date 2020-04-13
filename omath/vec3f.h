
#pragma once

#include "common.h"

typedef struct vec3f {
	float x;
	float y;
	float z;
} vec3f;

extern vec3f* vec3f_set( vec3f* out, const float x, const float y, const float z );

extern vec3f* vec3f_set_from( vec3f* out, const vec3f* const other );

extern vec3f* vec3f_sub( vec3f* out, const vec3f* const a, const vec3f* const b );

extern float vec3f_magnitude( const vec3f* const a );

extern float vec3f_dot( const vec3f* const a, const vec3f* const b );

extern vec3f* vec3f_normalize( vec3f* out, const vec3f* const a );

extern vec3f* vec3f_cross( vec3f* out, const vec3f* const a, const vec3f* const b );
