
#pragma once

#include "mat3d.h"
#include "quatd.h"

typedef struct {
	double data[16];
} mat4d;

// Extracts basis as 3x3 matrix.
extern void mat4d_get_basis( mat3d* out, const mat4d* const in );

extern mat4d* mat4d_from( mat4d* out, const mat4d* const other );

// Returns matrix component at specified row and column.
extern double mat4d_at( const mat4d* const m, size_t row, size_t column );

// Fills main diagonal of 'out' with 1.0, other elements becomes zero
extern void mat4d_identity( mat4d* out );

extern void mat4d_transpose( mat4d* out, const mat4d* const m );

extern void mat4d_mul( mat4d* out, const mat4d* const a, const mat4d* const b );

// Builds scaling matrix
extern void mat4d_scale( mat4d* out, const vec3d* const v );

// Builds perspective matrix
extern void mat4d_perspective( mat4d* out, const double fov_radians,
		const double aspect, const double zNear, const double zFar );

// Builds matrix for orthographics projection
extern void mat4d_ortho( mat4d* out, const double left, const double right,
		const double bottom, const double top, const double zNear, const double zFar );

// Builds translation matrix
extern mat4d* mat4d_translate( mat4d* out, const vec3d* const v );

// Builds "look-at" view matrix
extern void mat4d_lookat( mat4d* out, const vec3d* const eye, const vec3d* const at, const vec3d* const up );

// Inverses matrix, so A * A_inv = Identity
extern void mat4d_inverse( mat4d* out, const mat4d* const a );

// Build rotate matrix from quaternion
//extern void mat4d_rotate( mat4d* out, const struct quatd* q );
// Rotation matrix from axis vector (unnormalized) and angle
//extern void mat4d_rotate( mat4d* out, const double angle, const vec3d* axis );
// Could also be an axis rotation angle * vector( yaw, pitch, roll ) in model coords
// Axis angles in radians, pls !
extern mat4d* mat4d_rotate( mat4d* out, const double yaw_x, const double pitch_y, const double roll_z );

// Builds rotate matrix around (1, 0, 0) axis
extern void mat4d_rotate_x( mat4d* out, const double angle_radians );

// Builds rotate matrix around (0, 1, 0) axis
extern void mat4d_rotate_y( mat4d* out, const double angle_radians );

// Builds rotate matrix around (0, 0, 1) axis
extern void mat4d_rotate_z( mat4d* out, const double angle_radians );

// Extracts "up" vector from basis of matrix m
extern vec3d* mat4d_up( const mat4d* const m, vec3d* up );

// Extracts "side" vector from basis of matrix m @todo right side ?
extern vec3d* mat4d_side( const mat4d* const m, vec3d* side );

// Extracts "front" vector from basis of matrix m
extern vec3d* mat4d_look( const mat4d* const m, vec3d* look );

extern void mat4d_get_translation( const mat4d* const m, vec3d* translation );

extern quatd* mat4d_to_quat( const mat4d* const m, quatd* quat );

extern void mat4d_print( const mat4d* const m );

extern void mat4d_get_data_as_float( float *f, const mat4d* const m );
