#pragma once

#include "vector.hpp"
class Matrix;

class Plane
{
public:
	FORCEINLINE Plane();
	FORCEINLINE Plane(const Vector& plane);
	FORCEINLINE Plane(float x, float y, float z, float w);
	FORCEINLINE Plane(const Vector3f& normal, float w);
	Plane transform(const Matrix& transform) const;

	FORCEINLINE float dot(const Vector3f& other) const;
	FORCEINLINE float dotVector(const Vector& other) const;
	FORCEINLINE float dotPlane(const Plane& other) const;
	FORCEINLINE Plane normalized(float errorMargin=1.e-8f) const;
	FORCEINLINE bool isNormalized(float errorMargin=1.e-4f) const;

	FORCEINLINE Vector3f getNormal() const;
	FORCEINLINE Vector3f reflect(const Vector3f& point) const;

	bool intersectPlanes(Vector3f& intersectionPoint,
			const Plane& other1, const Plane& other2,
			float errormargin=1.e-8f) const;
	FORCEINLINE float intersectLine(const Vector3f& lineStart, const Vector3f& lineEnd) const;
	FORCEINLINE float intersectRay(const Vector3f& start, const Vector3f& rayDir) const;

	FORCEINLINE Plane abs() const;

	FORCEINLINE Plane operator-() const;
	FORCEINLINE Plane operator+(const Plane& other) const;
	FORCEINLINE Plane operator-(const Plane& other) const;
	FORCEINLINE Plane operator*(const Plane& other) const;
	FORCEINLINE Plane operator/(const Plane& other) const;

	FORCEINLINE Plane operator+=(const Plane& other);
	FORCEINLINE Plane operator-=(const Plane& other);
	FORCEINLINE Plane operator*=(const Plane& other);
	FORCEINLINE Plane operator/=(const Plane& other);

	FORCEINLINE Plane operator*(float amt) const;
	FORCEINLINE Plane operator/(float amt) const;
	FORCEINLINE Plane operator*=(float val);
	FORCEINLINE Plane operator/=(float val);

	FORCEINLINE bool operator==(const Plane& other) const;
	FORCEINLINE bool operator!=(const Plane& other) const;
	FORCEINLINE bool equals(const Plane& other, float errorMargin=1.e-4f) const;
	FORCEINLINE Vector toVector() const;
private:
	Vector data;
};

FORCEINLINE Plane::Plane() {}

FORCEINLINE Plane::Plane(const Vector& plane) :
	data(plane) {}

FORCEINLINE Plane::Plane(float x, float y, float z, float w) :
	data(Vector::make(x, y, z, w)) {}

FORCEINLINE Plane::Plane(const Vector3f& normal, float w) :
	data(normal.toVector(w)) {}

FORCEINLINE float Plane::dot(const Vector3f& other) const
{
	return other.toVector(1.0f).dot4(data)[0];
}

FORCEINLINE float Plane::dotVector(const Vector& other) const
{
	return other.dot4(data)[0];
}

FORCEINLINE float Plane::dotPlane(const Plane& other) const
{
	return other.data.dot4(data)[0];
}

FORCEINLINE Plane Plane::normalized(float errorMargin) const
{
	(void)errorMargin;
	return Plane(data.normalize3());
}

FORCEINLINE bool Plane::isNormalized(float errorMargin) const
{
	return Math::abs(1.0f - (data.dot3(data)[0])) < errorMargin;
}

FORCEINLINE Vector3f Plane::getNormal() const
{
	return Vector3f(data.normalize3());
}

FORCEINLINE float Plane::intersectLine(const Vector3f& lineStart, const Vector3f& lineEnd) const
{
	return intersectRay(lineStart, lineEnd-lineStart);
}

FORCEINLINE float Plane::intersectRay(const Vector3f& start, const Vector3f& rayDir) const
{
	return -dot(start)/rayDir.dot(getNormal());
}

FORCEINLINE Vector3f Plane::reflect(const Vector3f& point) const
{
	Vector pointAsVector(point.toVector(1.0f));
	Vector dotAmt = VectorConstants::TWO * data.dot4(pointAsVector);
	return Vector3f(pointAsVector - (data * dotAmt));
}

FORCEINLINE Plane Plane::abs() const
{
	return Plane(data.abs());
}

FORCEINLINE Plane Plane::operator-() const
{
	return Plane(-data);
}

FORCEINLINE Plane Plane::operator+(const Plane& other) const
{
	return Plane(data + other.data);
}

FORCEINLINE Plane Plane::operator-(const Plane& other) const
{
	return Plane(data - other.data);
}

FORCEINLINE Plane Plane::operator*(const Plane& other) const
{
	return Plane(data * other.data);
}

FORCEINLINE Plane Plane::operator/(const Plane& other) const
{
	return Plane(data / other.data);
}


FORCEINLINE Plane Plane::operator+=(const Plane& other)
{
	data = data + other.data;
	return *this;
}

FORCEINLINE Plane Plane::operator-=(const Plane& other)
{
	data = data - other.data;
	return *this;
}

FORCEINLINE Plane Plane::operator*=(const Plane& other)
{
	data = data * other.data;
	return *this;
}

FORCEINLINE Plane Plane::operator/=(const Plane& other)
{
	data = data / other.data;
	return *this;
}


FORCEINLINE Plane Plane::operator*(float amt) const
{
	return Plane(data * Vector::load1f(amt));
}

FORCEINLINE Plane Plane::operator/(float amt) const
{
	return Plane(data * Vector::load1f(Math::reciprocal(amt)));
}

FORCEINLINE Plane Plane::operator*=(float val)
{
	data = data * Vector::load1f(val);
	return *this;
}

FORCEINLINE Plane Plane::operator/=(float val)
{
	data = data * Vector::load1f(Math::reciprocal(val));
	return *this;
}


FORCEINLINE bool Plane::operator==(const Plane& other) const
{
	return (data != other.data).isZero4f();
}

FORCEINLINE bool Plane::operator!=(const Plane& other) const
{
	return (data == other.data).isZero4f();
}

FORCEINLINE bool Plane::equals(const Plane& other, float errorMargin) const
{
	return data.notEquals(other.data, errorMargin).isZero4f();
}

FORCEINLINE Vector Plane::toVector() const
{
	return data;
}
