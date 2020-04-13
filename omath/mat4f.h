
#pragma once

//#include "mat3f.h"
//#include "quatf.h"
#include "vec3f.h"

typedef struct {
	float data[16];
} mat4f;

// Extracts basis as 3x3 matrix.
//extern void mat4f_get_basis( mat3d* out, const mat4f* const in );

extern mat4f* mat4f_from( mat4f* out, const mat4f* const other );

// Returns matrix component at specified row and column.
extern float mat4f_at( const mat4f* const m, size_t row, size_t column );

// Fills main diagonal of 'out' with 1.0, other elements becomes zero
extern void mat4f_identity( mat4f* out );

extern void mat4f_transpose( mat4f* out, const mat4f* const m );

extern void mat4f_mul( mat4f* out, const mat4f* const a, const mat4f* const b );

// Builds scaling matrix
extern void mat4f_scale( mat4f* out, const vec3f* const v );

// Builds perspective matrix
extern void mat4f_perspective( mat4f* out, const float fov_radians,
		const float aspect, const float zNear, const float zFar );

// Builds matrix for orthographics projection
extern void mat4f_ortho( mat4f* out, const float left, const float right,
		const float bottom, const float top, const float zNear, const float zFar );

// Builds translation matrix
extern mat4f* mat4f_translate( mat4f* out, const vec3f* const v );

// Builds "look-at" view matrix
extern void mat4f_lookat( mat4f* out, const vec3f* const eye, const vec3f* const at, const vec3f* const up );

// Inverses matrix, so A * A_inv = Identity
extern void mat4f_inverse( mat4f* out, const mat4f* const a );

// Build rotate matrix from quaternion
//extern void mat4f_rotate( mat4f* out, const struct quatd* q );
// Rotation matrix from axis vector (unnormalized) and angle
//extern void mat4f_rotate( mat4f* out, const float angle, const vec3f* axis );
// Could also be an axis rotation angle * vector( yaw, pitch, roll ) in model coords
// Axis angles in radians, pls !
extern mat4f* mat4f_rotate( mat4f* out, const float yaw_x, const float pitch_y, const float roll_z );

// Builds rotate matrix around (1, 0, 0) axis
extern void mat4f_rotate_x( mat4f* out, const float angle_radians );

// Builds rotate matrix around (0, 1, 0) axis
extern void mat4f_rotate_y( mat4f* out, const float angle_radians );

// Builds rotate matrix around (0, 0, 1) axis
extern void mat4f_rotate_z( mat4f* out, const float angle_radians );

// Extracts "up" vector from basis of matrix m
extern vec3f* mat4f_up( const mat4f* const m, vec3f* up );

// Extracts "side" vector from basis of matrix m @todo right side ?
extern vec3f* mat4f_side( const mat4f* const m, vec3f* side );

// Extracts "front" vector from basis of matrix m
extern vec3f* mat4f_look( const mat4f* const m, vec3f* look );

extern void mat4f_get_translation( const mat4f* const m, vec3f* translation );

//extern quatd* mat4f_to_quat( const mat4f* const m, quatd* quat );

extern void mat4f_print( const mat4f* const m );
