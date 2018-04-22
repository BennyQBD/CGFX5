#pragma once

#include "vector.hpp"
#include "matrix.hpp"

class Sphere
{
public:
	FORCEINLINE Sphere() {}
	FORCEINLINE Sphere(const Vector3f& center, float radius) :
		data(center.toVector(radius)) {}
	Sphere(Vector3f* points, uint32 amt);
	Sphere(float* points, uint32 amt);

	bool intersectRay(const Vector3f& start, const Vector3f& rayDir, float& point1, float& point2) const;
	bool intersectLine(const Vector3f& start, const Vector3f& end) const;
	FORCEINLINE bool intersects(const Sphere& other, float errorMargin=1.e-4f) const;
	Sphere transform(const Matrix& transform) const;
	FORCEINLINE Sphere expand(float distance) const;
	FORCEINLINE Sphere moveTo(const Vector3f& destination) const;
	FORCEINLINE float getVolume() const;
	FORCEINLINE float getRadius() const;
	FORCEINLINE Vector3f getCenter() const;
	FORCEINLINE bool contains(const Vector3f& point, float errorMargin=1.e-4f) const;
	FORCEINLINE bool contains(const Sphere& other, float errorMargin=1.e-4f) const;
	FORCEINLINE Sphere translate(const Vector3f& amt) const;
	FORCEINLINE Sphere scaleFromCenter(float amt) const;
	FORCEINLINE Sphere scaleFromOrigin(float amt) const;
	FORCEINLINE Sphere addPoint(const Vector3f& other) const;
	Sphere addSphere(const Sphere& other) const;
	FORCEINLINE Vector toVector() const;

	FORCEINLINE bool operator==(const Sphere& other) const;
	FORCEINLINE bool operator!=(const Sphere& other) const;
	FORCEINLINE bool equals(const Sphere& other, float errorMargin=1.e-4f) const;
private:
	Vector data;

	FORCEINLINE Sphere(const Vector& newData) :
		data(newData) {}
};

FORCEINLINE bool Sphere::intersects(const Sphere& other, float errorMargin) const
{
	Vector dist = data - other.data;
	float maxR = Math::max(0.0f, data[3] + other.data[3] + errorMargin);
	return dist.dot3(dist)[0] < maxR*maxR;
}

FORCEINLINE Sphere Sphere::expand(float distance) const
{
	return Sphere(data + Vector::make(0.0f,0.0f,0.0f,distance));
}

FORCEINLINE Sphere Sphere::moveTo(const Vector3f& destination) const
{
	return Sphere(destination, data[3]);
}

FORCEINLINE float Sphere::getVolume() const
{
	float radius = data[3];
	return MATH_PI*4.0f/3.0f * radius*radius*radius;
}

FORCEINLINE float Sphere::getRadius() const
{
	return data[3];
}

FORCEINLINE Vector3f Sphere::getCenter() const
{
	return Vector3f(data);
}

FORCEINLINE bool Sphere::contains(const Vector3f& point, float errorMargin) const
{
	float r = data[3]+errorMargin;
	return point.distSquared(Vector3f(data)) < r*r;
}

FORCEINLINE bool Sphere::contains(const Sphere& other, float errorMargin) const
{
	// If other sphere is bigger than this, must return false.
	// Otherwise, false positives since the rest of the equation is direction
	// independent
	if(data[3] < other.data[3]+errorMargin) {
		return false;
	}
	Vector dataDist = data-other.data;
	float r = errorMargin+dataDist[3];
	float dSq = dataDist.dot3(dataDist)[0];
	return dSq < r*r;
}

FORCEINLINE Sphere Sphere::translate(const Vector3f& amt) const
{
	return Sphere(data + amt.toVector(0.0f));
}

FORCEINLINE Sphere Sphere::scaleFromCenter(float amt) const
{
	return Sphere(data * Vector::make(1.0f,1.0f,1.0f,amt));
}

FORCEINLINE Sphere Sphere::scaleFromOrigin(float amt) const
{
	return Sphere(data * Vector::make(amt,amt,amt,amt));
}

FORCEINLINE Sphere Sphere::addPoint(const Vector3f& other) const
{
	return addSphere(Sphere(other, 0.0f));
}

FORCEINLINE bool Sphere::operator==(const Sphere& other) const
{
	return (data != other.data).isZero4f();
}

FORCEINLINE bool Sphere::operator!=(const Sphere& other) const
{
	return (data == other.data).isZero4f();
}

FORCEINLINE bool Sphere::equals(const Sphere& other, float errorMargin) const
{
	return data.notEquals(other.data, errorMargin).isZero4f();
}

FORCEINLINE Vector Sphere::toVector() const
{
	return data;
}

