#include "stdafx.h"
#include <math.h>
#include "V3.h"


const V3 V3::Zero(0.0, 0.0, 0.0);
const V3 V3::X(1.0, 0.0, 0.0);
const V3 V3::Y(0.0, 1.0, 0.0);
const V3 V3::Z(0.0, 0.0, 1.0);


V3::V3(void)
{
	set(0, 0, 0);
}

V3::V3(float x, float y, float z)
{
	set(x, y, z);
}


V3::V3(const float *v)
{
	set(v[0], v[1], v[2]);
}


V3::V3(const V3 &v)
{
	set(v.vec[0], v.vec[1], v.vec[2]);
}


V3& V3::operator= (const V3& v)
{
	set(v.vec[0], v.vec[1], v.vec[2]);
	return *this;
}


void V3::set(float x, float y, float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}


float& V3::operator[](int i)
{
	if ((i >= 0) && (i < 3))
		return vec[i];
	else if (i < 0)
		return vec[0];
	else return vec[2];
}


float V3::operator[](int i) const
{
	if ((i >= 0) && (i < 3))
		return vec[i];
	else if (i < 0)
		return vec[0];
	else return vec[2];
}


V3& V3::operator+=(const V3& v)
{
	vec[0] += v.vec[0];
	vec[1] += v.vec[1];
	vec[2] += v.vec[2];
	return *this;
}


V3& V3::operator-=(const V3& v)
{
	vec[0] -= v.vec[0];
	vec[1] -= v.vec[1];
	vec[2] -= v.vec[2];
	return *this;
}


V3& V3::operator*=(float s)
{
	vec[0] *= s;
	vec[1] *= s;
	vec[2] *= s;
	return *this;
}


V3& V3::operator/=(float s)
{
	vec[0] /= s;
	vec[1] /= s;
	vec[2] /= s;
	return *this;
}


V3 V3::operator+(const V3& v) const
{
	return V3(vec[0]+v.vec[0], vec[1]+v.vec[1], vec[2]+v.vec[2]);
}


V3 V3::operator-(const V3& v) const
{
	return V3(vec[0]-v.vec[0], vec[1]-v.vec[1], vec[2]-v.vec[2]);
}


V3 V3::operator-(void) const
{
	return V3(-vec[0],-vec[1], -vec[2]);
}


V3 V3::operator*(float s) const
{
	return V3(vec[0]*s, vec[1]*s, vec[2]*s);
}


V3 V3::operator/(float s) const
{
	return V3(vec[0]/s, vec[1]/s, vec[2]/s);
}


bool V3::operator==(const V3& v) const
{
	return ((vec[0] == v.vec[0]) &&
			(vec[1] == v.vec[1]) &&
			(vec[2] == v.vec[2]));
}


bool V3::operator!=(const V3& v) const
{
	return ((vec[0] != v.vec[0]) ||
			(vec[1] != v.vec[1]) ||
			(vec[2] != v.vec[2]));
}

    
float V3::length(void) const
{
	return sqrt(lengthSquared());
}


float V3::lengthSquared(void) const
{
	return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
}


void V3::normalize(void)
{
	float len = length();
	if (len > 0)
		{
		vec[0] /= len;
		vec[1] /= len;
		vec[2] /= len;
		}
}


float V3::dot(const V3& v) const
{
	return vec[0]*v.vec[0] + vec[1]*v.vec[1] + vec[2]*v.vec[2];
}


V3 V3::cross(const V3& v) const
{
	return V3(vec[1] * v.vec[2] - vec[2] * v.vec[1],
				   vec[2] * v.vec[0] - vec[0] * v.vec[2],
				   vec[0] * v.vec[1] - vec[1] * v.vec[0]);
}


float V3::distance(const V3& v) const
{
	return sqrt(distanceSquared(v));
}


float V3::distanceSquared(const V3& v) const
{
	float dx, dy, dz;
	dx = vec[0] - v.vec[0];
	dy = vec[1] - v.vec[1];
	dz = vec[2] - v.vec[2];
	return dx*dx + dy*dy + dz*dz;
}
