
#pragma once

#include "common.h"

typedef struct vec2d {
	double x;
	double y;
} vec2d;

extern vec2d* vec2d_set( vec2d* out, const double x, const double y );

extern vec2d* vec2d_set_from( vec2d* out, const vec2d* const other );
