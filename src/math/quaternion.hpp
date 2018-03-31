#pragma once

#include "vector.hpp"

class Quaternion
{
public:
	inline Quaternion() {}
	inline Quaternion(const Vector& quat);
	inline Quaternion(float x, float y, float z, float w);
	inline Quaternion(const Vector3f& axis, float angle);
	
	inline float operator[](uint32 index) const;
	inline Quaternion operator+(const Quaternion& other) const;
	inline Quaternion operator+=(const Quaternion& other);
	inline Quaternion operator-(const Quaternion& other) const;
	inline Quaternion operator-=(const Quaternion& other);
	inline Quaternion operator*(const Quaternion& other) const;
	inline Quaternion operator*=(const Quaternion& other);
	inline Vector3f operator*(const Vector3f& other) const;
	inline Quaternion operator*(float amt) const;
	inline Quaternion operator*=(float amt);
	inline Quaternion operator/(float amt) const;
	inline Quaternion operator/=(float amt);

	inline bool operator==(const Quaternion& other) const;
	inline bool operator!=(const Quaternion& other) const;
	inline bool equals(const Quaternion& other, float errorMargin=1.e-4f) const;

	inline float dot(const Quaternion& other) const;

	inline float length() const;
	inline float lengthSquared() const;

	Quaternion normalized(float errorMargin=1.e-8f) const;
	bool isNormalized(float errorMargin=1.e-4f) const;

	Vector3f getAxis() const;
	float getAngle() const;
	void axisAndAngle(Vector3f& axis, float& angle) const;

	Vector3f rotate(const Vector3f& other) const;
	Quaternion slerp(const Quaternion& dest, float amt, float errorMargin=1.e-4f) const;
	Quaternion conjugate() const;
	Quaternion inverse() const;

	inline Vector3f getAxisX() const;
	inline Vector3f getAxisY() const;
	inline Vector3f getAxisZ() const;
	inline Vector toVector() const;
private:
	Vector vec;
};

template<>
inline Quaternion Math::lerp(const Quaternion& val1, const Quaternion& val2,
		const float& amt)
{
	float dotAmt = val1.dot(val2);
	float dir = Math::select(dotAmt, 1.0f, -1.0f);
	return (val2 * amt) + val1 * (dir * (1.0f - amt));
}

inline Quaternion::Quaternion(const Vector& quat) :
	vec(quat) {}

inline Quaternion::Quaternion(float x, float y, float z, float w) :
	vec(Vector::make(x, y, z, w)) {}

inline Quaternion::Quaternion(const Vector3f& axis, float angle)
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

inline float Quaternion::operator[](uint32 index) const
{
	return vec[index];
}

inline Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return Quaternion(vec + other.vec);
}

inline Quaternion Quaternion::operator+=(const Quaternion& other)
{
	vec = vec + other.vec;
	return *this;
}

inline Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return Quaternion(vec - other.vec);
}

inline Quaternion Quaternion::operator-=(const Quaternion& other)
{
	vec = vec - other.vec;
	return *this;
}

inline Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return Quaternion(vec.quatMul(other.vec));	
}

inline Quaternion Quaternion::operator*=(const Quaternion& other)
{
	vec = vec.quatMul(other.vec);
	return *this;
}

inline Vector3f Quaternion::operator*(const Vector3f& other) const
{
	return rotate(other);
}

inline Quaternion Quaternion::operator*(float amt) const
{
	return Quaternion(vec * Vector::load1f(amt));
}

inline Quaternion Quaternion::operator*=(float amt) 
{
	vec = vec * Vector::load1f(amt);
	return *this;
}

inline Quaternion Quaternion::operator/(float amt) const
{
	return *this * Math::reciprocal(amt);
}

inline Quaternion Quaternion::operator/=(float amt)
{
	return *this *= Math::reciprocal(amt);
}


inline bool Quaternion::operator==(const Quaternion& other) const
{
	return (vec != other.vec).isZero4f();
}

inline bool Quaternion::operator!=(const Quaternion& other) const
{
	return (vec == other.vec).isZero4f();
}

inline bool Quaternion::equals(const Quaternion& other, float errorMargin) const
{
	Vector error = Vector::load1f(errorMargin);
	return ((vec - other.vec).abs() >= error).isZero4f() ||
		((vec + other.vec).abs() >= error).isZero4f();
}


inline float Quaternion::dot(const Quaternion& other) const
{
	return vec.dot4(other.vec)[0];
}


inline float Quaternion::length() const
{
	return Math::sqrt(lengthSquared());
}

inline float Quaternion::lengthSquared() const
{
	return vec.dot4(vec)[0];
}

inline Vector3f Quaternion::getAxisX() const
{
	return rotate(Vector3f(1.0f, 0.0f, 0.0f));
}

inline Vector3f Quaternion::getAxisY() const
{
	return rotate(Vector3f(0.0f, 1.0f, 0.0f));
}

inline Vector3f Quaternion::getAxisZ() const
{
	return rotate(Vector3f(0.0f, 0.0f, 1.0f));
}

inline Vector Quaternion::toVector() const
{
	return vec;
}
