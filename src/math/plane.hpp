#pragma once

#include "vector.hpp"
class Matrix;

class Plane
{
public:
	inline Plane();
	inline Plane(const Vector& plane);
	inline Plane(float x, float y, float z, float w);
	inline Plane(const Vector3f& normal, float w);
	Plane transform(const Matrix& transform) const;

	inline float dot(const Vector3f& other) const;
	inline float dotVector(const Vector& other) const;
	inline float dotPlane(const Plane& other) const;
	inline Plane normalized(float errorMargin=1.e-8f) const;
	inline bool isNormalized(float errorMargin=1.e-4f) const;

	inline Vector3f getNormal() const;
	inline Vector3f reflect(const Vector3f& point) const;

	bool intersectPlanes(Vector3f& intersectionPoint,
			const Plane& other1, const Plane& other2,
			float errormargin=1.e-8f) const;
	inline float intersectLine(const Vector3f& lineStart, const Vector3f& lineEnd) const;
	inline float intersectRay(const Vector3f& start, const Vector3f& rayDir) const;

	inline Plane abs() const;

	inline Plane operator-() const;
	inline Plane operator+(const Plane& other) const;
	inline Plane operator-(const Plane& other) const;
	inline Plane operator*(const Plane& other) const;
	inline Plane operator/(const Plane& other) const;

	inline Plane operator+=(const Plane& other);
	inline Plane operator-=(const Plane& other);
	inline Plane operator*=(const Plane& other);
	inline Plane operator/=(const Plane& other);

	inline Plane operator*(float amt) const;
	inline Plane operator/(float amt) const;
	inline Plane operator*=(float val);
	inline Plane operator/=(float val);

	inline bool operator==(const Plane& other) const;
	inline bool operator!=(const Plane& other) const;
	inline bool equals(const Plane& other, float errorMargin=1.e-4f) const;
	inline Vector toVector() const;
private:
	Vector data;
};

inline Plane::Plane() {}

inline Plane::Plane(const Vector& plane) :
	data(plane) {}

inline Plane::Plane(float x, float y, float z, float w) :
	data(Vector::make(x, y, z, w)) {}

inline Plane::Plane(const Vector3f& normal, float w) :
	data(normal.toVector(w)) {}

inline float Plane::dot(const Vector3f& other) const
{
	return other.toVector(1.0f).dot4(data)[0];
}

inline float Plane::dotVector(const Vector& other) const
{
	return other.dot4(data)[0];
}

inline float Plane::dotPlane(const Plane& other) const
{
	return other.data.dot4(data)[0];
}

inline Plane Plane::normalized(float errorMargin) const
{
	(void)errorMargin;
	return Plane(data.normalize3());
}

inline bool Plane::isNormalized(float errorMargin) const
{
	return Math::abs(1.0f - (data.dot3(data)[0])) < errorMargin;
}

inline Vector3f Plane::getNormal() const
{
	return Vector3f(data.normalize3());
}

inline float Plane::intersectLine(const Vector3f& lineStart, const Vector3f& lineEnd) const
{
	return intersectRay(lineStart, lineEnd-lineStart);
}

inline float Plane::intersectRay(const Vector3f& start, const Vector3f& rayDir) const
{
	return -dot(start)/rayDir.dot(getNormal());
}

inline Vector3f Plane::reflect(const Vector3f& point) const
{
	Vector pointAsVector(point.toVector(1.0f));
	Vector dotAmt = VectorConstants::TWO * data.dot4(pointAsVector);
	return Vector3f(pointAsVector - (data * dotAmt));
}

inline Plane Plane::abs() const
{
	return Plane(data.abs());
}

inline Plane Plane::operator-() const
{
	return Plane(-data);
}

inline Plane Plane::operator+(const Plane& other) const
{
	return Plane(data + other.data);
}

inline Plane Plane::operator-(const Plane& other) const
{
	return Plane(data - other.data);
}

inline Plane Plane::operator*(const Plane& other) const
{
	return Plane(data * other.data);
}

inline Plane Plane::operator/(const Plane& other) const
{
	return Plane(data / other.data);
}


inline Plane Plane::operator+=(const Plane& other)
{
	data = data + other.data;
	return *this;
}

inline Plane Plane::operator-=(const Plane& other)
{
	data = data - other.data;
	return *this;
}

inline Plane Plane::operator*=(const Plane& other)
{
	data = data * other.data;
	return *this;
}

inline Plane Plane::operator/=(const Plane& other)
{
	data = data / other.data;
	return *this;
}


inline Plane Plane::operator*(float amt) const
{
	return Plane(data * Vector::load1f(amt));
}

inline Plane Plane::operator/(float amt) const
{
	return Plane(data * Vector::load1f(Math::reciprocal(amt)));
}

inline Plane Plane::operator*=(float val)
{
	data = data * Vector::load1f(val);
	return *this;
}

inline Plane Plane::operator/=(float val)
{
	data = data * Vector::load1f(Math::reciprocal(val));
	return *this;
}


inline bool Plane::operator==(const Plane& other) const
{
	return (data != other.data).isZero4f();
}

inline bool Plane::operator!=(const Plane& other) const
{
	return (data == other.data).isZero4f();
}

inline bool Plane::equals(const Plane& other, float errorMargin) const
{
	return data.notEquals(other.data, errorMargin).isZero4f();
}

inline Vector Plane::toVector() const
{
	return data;
}
