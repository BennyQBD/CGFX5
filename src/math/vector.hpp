#pragma once

#include "../core/common.hpp"
#include "vecmath.hpp"

class Vector3f
{
public:
	FORCEINLINE Vector3f();
	FORCEINLINE Vector3f(float val);
	FORCEINLINE Vector3f(float xIn, float yIn, float zIn);
	FORCEINLINE Vector3f(const Vector& vecIn);

	FORCEINLINE float dot(const Vector3f& other) const;
	FORCEINLINE Vector3f dotToVector(const Vector3f& other) const;
	FORCEINLINE Vector3f cross(const Vector3f& other) const;
	FORCEINLINE float dist(const Vector3f& other) const;
	FORCEINLINE float distSquared(const Vector3f& other) const;

	FORCEINLINE Vector3f operator+(const Vector3f& other) const;
	FORCEINLINE Vector3f operator-(const Vector3f& other) const;
	FORCEINLINE Vector3f operator*(const Vector3f& other) const;
	FORCEINLINE Vector3f operator/(const Vector3f& other) const;

	FORCEINLINE Vector3f operator+(float amt) const;
	FORCEINLINE Vector3f operator-(float amt) const;
	FORCEINLINE Vector3f operator*(float amt) const;
	FORCEINLINE Vector3f operator/(float amt) const;

	bool operator==(const Vector3f& other) const;
	bool operator!=(const Vector3f& other) const;
	bool equals(const Vector3f& other, float errorMargin=1.e-4f) const;
	bool equals(float val, float errorMargin=1.e-4f) const;

	FORCEINLINE Vector3f operator-() const;
	FORCEINLINE Vector3f operator+=(const Vector3f& other);
	FORCEINLINE Vector3f operator-=(const Vector3f& other);
	FORCEINLINE Vector3f operator*=(const Vector3f& other);
	FORCEINLINE Vector3f operator/=(const Vector3f& other);

	FORCEINLINE Vector3f operator+=(float val);
	FORCEINLINE Vector3f operator-=(float val);
	FORCEINLINE Vector3f operator*=(float val);
	FORCEINLINE Vector3f operator/=(float val);

	float operator[](uint32 index) const;

	void set(float x, float y, float z);
	void set(uint32 index, float val);
	float max() const;
	float min() const;
	float absMax() const;
	float absMin() const;

	Vector3f abs() const;
	Vector3f min(const Vector3f& other) const;
	Vector3f max(const Vector3f& other) const;

	FORCEINLINE float length() const;
	FORCEINLINE float lengthSquared() const;

	Vector3f normalized(float errorMargin=1.e-8f) const;
	bool isNormalized(float errorMargin=1.e-4f) const;

	void dirAndLength(Vector3f& dir, float& length) const;
	Vector3f project() const;
	Vector3f reciprocal() const;

	Vector3f rotate(const Vector3f& axis, float angle) const;

	Vector3f reflect(const Vector3f& normal) const;
	Vector3f refract(const Vector3f& normal, float indexOfRefraction) const;

	Vector3f toDegrees() const;
	Vector3f toRadians() const;

	FORCEINLINE Vector toVector() const;
	Vector toVector(float w) const;
private:
	Vector vec;
};

FORCEINLINE Vector3f::Vector3f(const Vector& vecIn) :
	vec(vecIn) {}

FORCEINLINE Vector3f::Vector3f() :
	vec(VectorConstants::ZERO) {}

FORCEINLINE Vector3f::Vector3f(float val) :
	vec(Vector::make(val, val, val, 0.0f)) {}

FORCEINLINE Vector3f::Vector3f(float xIn, float yIn, float zIn) :
	vec(Vector::make(xIn, yIn, zIn, 0.0f)) {}

FORCEINLINE float Vector3f::dot(const Vector3f& other) const
{
	return vec.dot3(other.vec)[0];
}

FORCEINLINE Vector3f Vector3f::dotToVector(const Vector3f& other) const
{
	return Vector3f(vec.dot3(other.vec));
}

FORCEINLINE Vector3f Vector3f::cross(const Vector3f& other) const
{
	return Vector3f(vec.cross3(other.vec));
}

FORCEINLINE float Vector3f::dist(const Vector3f& other) const
{
	return Math::sqrt(distSquared(other));
}

FORCEINLINE float Vector3f::distSquared(const Vector3f& other) const
{
	Vector3f temp = other - *this;
	return temp.vec.dot3(temp.vec)[0];
}


FORCEINLINE Vector3f Vector3f::operator+(const Vector3f& other) const
{
	return Vector3f(vec + other.vec);
}

FORCEINLINE Vector3f Vector3f::operator-(const Vector3f& other) const
{
	return Vector3f(vec - other.vec);
}

FORCEINLINE Vector3f Vector3f::operator*(const Vector3f& other) const
{
	return Vector3f(vec * other.vec);
}

FORCEINLINE Vector3f Vector3f::operator/(const Vector3f& other) const
{
	return Vector3f(vec / other.vec);
}


FORCEINLINE Vector3f Vector3f::operator+(float amt) const
{
	return Vector3f(vec + Vector::load1f(amt));
}

FORCEINLINE Vector3f Vector3f::operator-(float amt) const
{
	return Vector3f(vec - Vector::load1f(amt));
}

FORCEINLINE Vector3f Vector3f::operator*(float amt) const
{
	return Vector3f(vec * Vector::load1f(amt));
}

FORCEINLINE Vector3f Vector3f::operator/(float amt) const
{
	return Vector3f(vec * Vector::load1f(Math::reciprocal(amt)));
}


FORCEINLINE Vector3f Vector3f::operator-() const
{
	return Vector3f(-vec);
}

FORCEINLINE Vector3f Vector3f::operator+=(const Vector3f& other)
{
	vec = vec + other.vec;
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator-=(const Vector3f& other)
{
	vec = vec - other.vec;
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator*=(const Vector3f& other)
{
	vec = vec * other.vec;
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator/=(const Vector3f& other)
{
	vec = vec / other.vec;
	return *this;
}


FORCEINLINE float Vector3f::length() const
{
	return Math::sqrt(lengthSquared());
}

FORCEINLINE float Vector3f::lengthSquared() const
{
	return vec.dot3(vec)[0];
}


FORCEINLINE Vector3f Vector3f::operator+=(float val)
{
	vec = vec + Vector::load1f(val);
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator-=(float val)
{
	vec = vec - Vector::load1f(val);
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator*=(float val)
{
	vec = vec * Vector::load1f(val);
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator/=(float val)
{
	vec = vec * Vector::load1f(Math::reciprocal(val));
	return *this;
}

FORCEINLINE Vector Vector3f::toVector() const
{
	return vec;
}

