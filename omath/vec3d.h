
#pragma once

#include "common.h"

typedef struct {
	double x;
	double y;
	double z;
} vec3d;

extern vec3d* vec3d_set( vec3d* out, const double x, const double y, const double z );

extern vec3d* vec3d_set_from( vec3d* out, const vec3d* const other );

extern vec3d* vec3d_add( vec3d* out, const vec3d* const a, const vec3d* const b );

extern vec3d* vec3d_sub( vec3d* out, const vec3d* const a, const vec3d* const b );

extern vec3d* vec3d_div( vec3d* out, const vec3d* const a, const vec3d* const b );

extern vec3d* vec3d_mul( vec3d* out, const vec3d* const a, const vec3d* const b );

extern vec3d* vec3d_mul_s( vec3d* out, const vec3d* const a, const double scalar );

extern vec3d* vec3d_negate( vec3d* out, const vec3d* const a );

extern vec3d* vec3d_scale( vec3d* out, const vec3d* const a, const double s );

extern double vec3d_dot( const vec3d* const a, const vec3d* const b );

extern vec3d* vec3d_cross( vec3d* out, const vec3d* const a, const vec3d* const b );

extern double vec3d_magnitude( const vec3d* const a );

extern double vec3d_magnitude_sq( const vec3d* const a );

// Normalizes. Set ENABLE_SAFETY_CHECKS to cause assertion fail for 0 length.
extern vec3d* vec3d_normalize( vec3d* out, const vec3d* const a );

extern vec3d* vec3d_lerp( vec3d* out, const vec3d* const a, const vec3d* const b, double t );

extern double vec3d_distance( const vec3d* const a, const vec3d* const b );

extern double vec3d_distance_sq( const vec3d* const a, const vec3d* const b );

// Calculates angle in radians
extern double vec3d_angle( const vec3d* const a, const vec3d* const b );

// Projects point onto plane using normal vector of the plane
extern vec3d* vec3d_project_plane( vec3d* out, const vec3d* const point, const vec3d* const normal );

// Calculates reflection vector of a using normal vector of plane
extern vec3d* vec3d_reflect( vec3d* out, const vec3d* const a, const vec3d* const normal );

// Calculates middle point between two points
extern vec3d* vec3d_middle( vec3d* out, const vec3d* const a, const vec3d* const b );

// Calculates per-component min-max values. Can be used to calculate bounds of cloud of points
extern void vec3d_min_max( const vec3d* const a, vec3d* vMin, vec3d* vMax );

extern bool vec3d_fcmp( const vec3d* const a, const vec3d* const b );

extern bool vec3d_same_direction( const vec3d* const a, const vec3d* const b );

//bool vec3dry_normalize( vec3d* out, const vec3d* a);

//vec3d* de_vec3_normalize_ex( vec3d* out, const vec3d* a, float* length);

// Calculates new position of point a using matrix mat.
// multiplies 4-D vector (a->x, a->y, a->z, 1.0) using matrix mat.
//vec3d* vec3d_transform( vec3d* out, const vec3d* a, const de_mat4_t* mat);

// Calculates new direction of vector @a using matrix @mat (multiplies vector using 3*3-matrix basis)
//vec3d* vec3d_transform_normal( vec3d* out, const vec3d* a, const de_mat4_t* mat);

extern void vec3d_print( const vec3d* const v );

