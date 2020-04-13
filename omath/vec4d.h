
#pragma once

typedef struct {
	double x;
	double y;
	double z;
	double w;
} vec4d;

extern vec4d* vec4d_set( vec4d* out, const double x, const double y, const double z, const double w );
