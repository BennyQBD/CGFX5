#pragma once

#include "vector.hpp"
#include "matrix.hpp"

class Sphere
{
public:
	inline Sphere() {}
	inline Sphere(const Vector3f& center, float radius) :
		data(center.toVector(radius)) {}
	Sphere(Vector3f* points, uint32 amt);
	Sphere(float* points, uint32 amt);

	bool intersectRay(const Vector3f& start, const Vector3f& rayDir, float& point1, float& point2) const;
	bool intersectLine(const Vector3f& start, const Vector3f& end) const;
	inline bool intersects(const Sphere& other, float errorMargin=1.e-4f) const;
	Sphere transform(const Matrix& transform) const;
	inline Sphere expand(float distance) const;
	inline Sphere moveTo(const Vector3f& destination) const;
	inline float getVolume() const;
	inline float getRadius() const;
	inline Vector3f getCenter() const;
	inline bool contains(const Vector3f& point, float errorMargin=1.e-4f) const;
	inline bool contains(const Sphere& other, float errorMargin=1.e-4f) const;
	inline Sphere translate(const Vector3f& amt) const;
	inline Sphere scaleFromCenter(float amt) const;
	inline Sphere scaleFromOrigin(float amt) const;
	inline Sphere addPoint(const Vector3f& other) const;
	Sphere addSphere(const Sphere& other) const;
	inline Vector toVector() const;

	inline bool operator==(const Sphere& other) const;
	inline bool operator!=(const Sphere& other) const;
	inline bool equals(const Sphere& other, float errorMargin=1.e-4f) const;
private:
	Vector data;

	inline Sphere(const Vector& newData) :
		data(newData) {}
};

inline bool Sphere::intersects(const Sphere& other, float errorMargin) const
{
	Vector dist = data - other.data;
	float maxR = Math::max(0.0f, data[3] + other.data[3] + errorMargin);
	return dist.dot3(dist)[0] < maxR*maxR;
}

inline Sphere Sphere::expand(float distance) const
{
	return Sphere(data + Vector::make(0.0f,0.0f,0.0f,distance));
}

inline Sphere Sphere::moveTo(const Vector3f& destination) const
{
	return Sphere(destination, data[3]);
}

inline float Sphere::getVolume() const
{
	float radius = data[3];
	return Math::PI*4.0f/3.0f * radius*radius*radius;
}

inline float Sphere::getRadius() const
{
	return data[3];
}

inline Vector3f Sphere::getCenter() const
{
	return Vector3f(data);
}

inline bool Sphere::contains(const Vector3f& point, float errorMargin) const
{
	float r = data[3]+errorMargin;
	return point.distSquared(Vector3f(data)) < r*r;
}

inline bool Sphere::contains(const Sphere& other, float errorMargin) const
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

inline Sphere Sphere::translate(const Vector3f& amt) const
{
	return Sphere(data + amt.toVector(0.0f));
}

inline Sphere Sphere::scaleFromCenter(float amt) const
{
	return Sphere(data * Vector::make(1.0f,1.0f,1.0f,amt));
}

inline Sphere Sphere::scaleFromOrigin(float amt) const
{
	return Sphere(data * Vector::make(amt,amt,amt,amt));
}

inline Sphere Sphere::addPoint(const Vector3f& other) const
{
	return addSphere(Sphere(other, 0.0f));
}

inline bool Sphere::operator==(const Sphere& other) const
{
	return (data != other.data).isZero4f();
}

inline bool Sphere::operator!=(const Sphere& other) const
{
	return (data == other.data).isZero4f();
}

inline bool Sphere::equals(const Sphere& other, float errorMargin) const
{
	return data.notEquals(other.data, errorMargin).isZero4f();
}

inline Vector Sphere::toVector() const
{
	return data;
}

