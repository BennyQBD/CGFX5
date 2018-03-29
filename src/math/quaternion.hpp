#pragma once

#include "vector.hpp"

class Quaternion
{
public:
	FORCEINLINE Quaternion() {}
	FORCEINLINE Quaternion(const Vector& quat);
	FORCEINLINE Quaternion(float x, float y, float z, float w);
	FORCEINLINE Quaternion(const Vector3f& axis, float angle);
	
	FORCEINLINE float operator[](uint32 index) const;
	FORCEINLINE Quaternion operator+(const Quaternion& other) const;
	FORCEINLINE Quaternion operator+=(const Quaternion& other);
	FORCEINLINE Quaternion operator-(const Quaternion& other) const;
	FORCEINLINE Quaternion operator-=(const Quaternion& other);
	FORCEINLINE Quaternion operator*(const Quaternion& other) const;
	FORCEINLINE Quaternion operator*=(const Quaternion& other);
	FORCEINLINE Vector3f operator*(const Vector3f& other) const;
	FORCEINLINE Quaternion operator*(float amt) const;
	FORCEINLINE Quaternion operator*=(float amt);
	FORCEINLINE Quaternion operator/(float amt) const;
	FORCEINLINE Quaternion operator/=(float amt);

	FORCEINLINE bool operator==(const Quaternion& other) const;
	FORCEINLINE bool operator!=(const Quaternion& other) const;
	FORCEINLINE bool equals(const Quaternion& other, float errorMargin=1.e-4f) const;

	FORCEINLINE float dot(const Quaternion& other) const;

	FORCEINLINE float length() const;
	FORCEINLINE float lengthSquared() const;

	Quaternion normalized(float errorMargin=1.e-8f) const;
	bool isNormalized(float errorMargin=1.e-4f) const;

	Vector3f getAxis() const;
	float getAngle() const;
	void axisAndAngle(Vector3f& axis, float& angle) const;

	Vector3f rotate(const Vector3f& other) const;
	Quaternion slerp(const Quaternion& dest, float amt, float errorMargin=1.e-4f) const;
	Quaternion conjugate() const;
	Quaternion inverse() const;

	FORCEINLINE Vector3f getAxisX() const;
	FORCEINLINE Vector3f getAxisY() const;
	FORCEINLINE Vector3f getAxisZ() const;
	FORCEINLINE Vector toVector() const;
private:
	Vector vec;
};

template<>
FORCEINLINE Quaternion Math::lerp(const Quaternion& val1, const Quaternion& val2,
		const float& amt)
{
	float dotAmt = val1.dot(val2);
	float dir = Math::select(dotAmt, 1.0f, -1.0f);
	return (val2 * amt) + val1 * (dir * (1.0f - amt));
}

FORCEINLINE Quaternion::Quaternion(const Vector& quat) :
	vec(quat) {}

FORCEINLINE Quaternion::Quaternion(float x, float y, float z, float w) :
	vec(Vector::make(x, y, z, w)) {}

FORCEINLINE Quaternion::Quaternion(const Vector3f& axis, float angle)
{
	float vals[3];
	float sinAngle, cosAngle;
	Math::sincos(&sinAngle, &cosAngle, angle * 0.5f);
	axis.toVector().store3f(vals);

	vec = Vector::make(
			vals[0] * sinAngle,
			vals[1] * sinAngle,
			vals[2] * sinAngle,
			cosAngle);
}

FORCEINLINE float Quaternion::operator[](uint32 index) const
{
	return vec[index];
}

FORCEINLINE Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return Quaternion(vec + other.vec);
}

FORCEINLINE Quaternion Quaternion::operator+=(const Quaternion& other)
{
	vec = vec + other.vec;
	return *this;
}

FORCEINLINE Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return Quaternion(vec - other.vec);
}

FORCEINLINE Quaternion Quaternion::operator-=(const Quaternion& other)
{
	vec = vec - other.vec;
	return *this;
}

FORCEINLINE Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return Quaternion(vec.quatMul(other.vec));	
}

FORCEINLINE Quaternion Quaternion::operator*=(const Quaternion& other)
{
	vec = vec.quatMul(other.vec);
	return *this;
}

FORCEINLINE Vector3f Quaternion::operator*(const Vector3f& other) const
{
	return rotate(other);
}

FORCEINLINE Quaternion Quaternion::operator*(float amt) const
{
	return Quaternion(vec * Vector::load1f(amt));
}

FORCEINLINE Quaternion Quaternion::operator*=(float amt) 
{
	vec = vec * Vector::load1f(amt);
	return *this;
}

FORCEINLINE Quaternion Quaternion::operator/(float amt) const
{
	return *this * Math::reciprocal(amt);
}

FORCEINLINE Quaternion Quaternion::operator/=(float amt)
{
	return *this *= Math::reciprocal(amt);
}


FORCEINLINE bool Quaternion::operator==(const Quaternion& other) const
{
	return (vec != other.vec).isZero4f();
}

FORCEINLINE bool Quaternion::operator!=(const Quaternion& other) const
{
	return (vec == other.vec).isZero4f();
}

FORCEINLINE bool Quaternion::equals(const Quaternion& other, float errorMargin) const
{
	Vector error = Vector::load1f(errorMargin);
	return ((vec - other.vec).abs() >= error).isZero4f() ||
		((vec + other.vec).abs() >= error).isZero4f();
}


FORCEINLINE float Quaternion::dot(const Quaternion& other) const
{
	return vec.dot4(other.vec)[0];
}


FORCEINLINE float Quaternion::length() const
{
	return Math::sqrt(lengthSquared());
}

FORCEINLINE float Quaternion::lengthSquared() const
{
	return vec.dot4(vec)[0];
}

FORCEINLINE Vector3f Quaternion::getAxisX() const
{
	return rotate(Vector3f(1.0f, 0.0f, 0.0f));
}

FORCEINLINE Vector3f Quaternion::getAxisY() const
{
	return rotate(Vector3f(0.0f, 1.0f, 0.0f));
}

FORCEINLINE Vector3f Quaternion::getAxisZ() const
{
	return rotate(Vector3f(0.0f, 0.0f, 1.0f));
}

FORCEINLINE Vector Quaternion::toVector() const
{
	return vec;
}
