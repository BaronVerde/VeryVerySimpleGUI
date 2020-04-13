
#pragma once

#include "common.h"

typedef struct vec2f {
	double x;
	double y;
} vec2f;

extern vec2f* vec2f_set( vec2f* out, const double x, const double y );

extern vec2f* vec2f_set_from( vec2f* out, const vec2f* const other );
