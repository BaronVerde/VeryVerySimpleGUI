
#pragma once

#include "vec3d.h"

// Possible rotation orders
typedef enum {
	DE_EULER_XYZ,
	DE_EULER_XZY,
	DE_EULER_YZX,
	DE_EULER_YXZ,
	DE_EULER_ZXY,
	DE_EULER_ZYX
} de_euler_t;

// Rotation quaternion
typedef struct {
	double x;
	double y;
	double z;
	double w;
} quatd;

// Directly sets quaternion components.
quatd* de_quat_set(quatd* out, double x, double y, double z, double w);

// Initializes quaternion using axis and angle (in radians)
quatd* de_quat_from_axis_angle(quatd* out, const vec3d* axis, double angle);

double de_quat_dot(const quatd* a, const quatd* b);

// Returns length of specified quaternion.
double de_quat_len(const quatd* a);

// Returns square length of specified quaternion.
double de_quat_sqr_len(const quatd* a);

// Returns angle between quaternions.
double de_quat_angle(const quatd* a, const quatd* b);

// Performs spherical interpolation between two quaternions
quatd* de_quat_slerp(quatd* out, const quatd* a, const quatd* b, double t);

// Initializes new quaternion using Euler angles with given order of rotation (XYZ, YXZ, etc)
void de_quat_from_euler(quatd* out, const vec3d* euler_radians, de_euler_t order);

// Multiplies two quaternions. Allows to combine rotations.
quatd* de_quat_mul(quatd* out, const quatd* a, const quatd* b);

// Normalizes quaternion.
quatd* de_quat_normalize(quatd* out, quatd* a);

// Returns main axis of quaternion.
vec3d* de_quat_get_axis(const quatd* q, vec3d* out_axis);

double de_quat_get_angle(const quatd* q);

// Checks equality of quaternions by per-component equality check.
bool de_quat_equals(const quatd* a, const quatd* b);

void de_quat_set_at(quatd * quat, double f, size_t index);
