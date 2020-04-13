
#pragma once

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4f;

extern vec4f* vec4f_set( vec4f* out, const float x, const float y, const float z, const float w );
