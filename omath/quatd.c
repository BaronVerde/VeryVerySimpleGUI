
#include "quatd.h"

quatd* de_quat_set(quatd* out, double x, double y, double z, double w)
{
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = w;
	return out;
}

quatd* de_quat_fraxis_angle(quatd* out, const vec3d* axis, double angle)
{
	double half_angle = angle * 0.5f;
	double d = vec3d_magnitude(axis);
	double s = sin(half_angle) / d;
	out->x = axis->x * s;
	out->y = axis->y * s;
	out->z = axis->z * s;
	out->w = cos(half_angle);
	return out;
}

double de_quat_dot(const quatd* a, const quatd* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

double de_quat_len(const quatd* a)
{
	return sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

double de_quat_sqr_len(const quatd* a)
{
	return a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w;
}

double de_quat_angle(const quatd* a, const quatd* b)
{
	double s = sqrt(de_quat_sqr_len(a) * de_quat_sqr_len(b));
	return acos(de_quat_dot(a, b) / s);
}

quatd* de_quat_slerp(quatd* out, const quatd* a, const quatd* b, double t)
{
	quatd q;
	double theta = de_quat_angle(a, b);
	if (fabs(theta) > 0.00001) {
		double d = 1.0 / sin(theta);
		double s0 = sin((1.0 - t) * theta);
		double s1 = sin(t * theta);
		if (de_quat_dot(a, b) < 0) {
			q.x = (a->x * s0 - b->x * s1) * d;
			q.y = (a->y * s0 - b->y * s1) * d;
			q.z = (a->z * s0 - b->z * s1) * d;
			q.w = (a->w * s0 - b->w * s1) * d;
		} else {
			q.x = (a->x * s0 + b->x * s1) * d;
			q.y = (a->y * s0 + b->y * s1) * d;
			q.z = (a->z * s0 + b->z * s1) * d;
			q.w = (a->w * s0 + b->w * s1) * d;
		}
		*out = q;
	} else {
		*out = *a;
	}
	return out;
}

void de_quat_freuler(quatd * out, const vec3d * euler_radians, de_euler_t order) {
	static vec3d x; vec3d_set( &x, 1, 0, 0 );
	static vec3d y; vec3d_set( &y, 0, 1, 0 );
	static vec3d z; vec3d_set( &z, 0, 0, 1 );
	quatd qx, qy, qz;
	de_quat_fraxis_angle(&qx, &x, euler_radians->x);
	de_quat_fraxis_angle(&qy, &y, euler_radians->y);
	de_quat_fraxis_angle(&qz, &z, euler_radians->z);
	switch (order) {
		case DE_EULER_XYZ:
			de_quat_mul(out, &qz, &qy);
			de_quat_mul(out, out, &qx);
			break;
		case DE_EULER_XZY:
			de_quat_mul(out, &qy, &qz);
			de_quat_mul(out, out, &qx);
			break;
		case DE_EULER_YZX:
			de_quat_mul(out, &qx, &qz);
			de_quat_mul(out, out, &qy);
			break;
		case DE_EULER_YXZ:
			de_quat_mul(out, &qz, &qx);
			de_quat_mul(out, out, &qy);
			break;
		case DE_EULER_ZXY:
			de_quat_mul(out, &qy, &qx);
			de_quat_mul(out, out, &qz);
			break;
		case DE_EULER_ZYX:
			de_quat_mul(out, &qx, &qy);
			de_quat_mul(out, out, &qz);
			break;
	}
}

quatd* de_quat_mul(quatd* out, const quatd* a, const quatd* b)
{
	quatd t;
	t.x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
	t.y = a->w * b->y + a->y * b->w + a->z * b->x - a->x * b->z;
	t.z = a->w * b->z + a->z * b->w + a->x * b->y - a->y * b->x;
	t.w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
	*out = t;
	return out;
}

quatd* de_quat_normalize(quatd* out, quatd* a)
{
	double k = 1.0 / de_quat_len(a);
	out->x *= k;
	out->y *= k;
	out->z *= k;
	out->w *= k;
	return out;
}

vec3d* de_quat_get_axis(const quatd* q, vec3d* out_axis ) {
	double s_squared = 1.0f - q->w * q->w;
	if (s_squared < 0.0001) {
		out_axis->x = 1.0; out_axis->y = 0.0; out_axis->z = 0.0;
	} else {
		double s = 1.0 / sqrt(s_squared);
		out_axis->x = q->x * s; out_axis->y = q->y * s; out_axis->z = q->z * s;
	}
	return out_axis;
}

double de_quat_get_angle(const quatd* q)
{
	return 2.0 * acos(q->w);
}

bool de_quat_equals(const quatd* a, const quatd* b)
{
	return a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w;
}

void de_quat_set_at( quatd* quat, double f, size_t index ) {
#if ENABLE_SAFETY_CHECKS
	assert( index < 4 );
#endif
	switch( index ) {
		case 0: quat->x = f; break;
		case 1: quat->y = f; break;
		case 2: quat->z = f; break;
		case 3: quat->w = f; break;
	}
}
