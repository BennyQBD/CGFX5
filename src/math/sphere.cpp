#include "sphere.hpp"

// According to the data in Thomas Larsson's Fast and Tight Fitting Bounding
// Spheres, doing naive algorithm with best of AABB and average centers tends
// to achieve very good results on average (average was between 2% and 3%)
// from optimal bounding sphere in their dataset.
// This was better than any of the "fast" algorithms combined, including
// Ritter's algorithm, though it did worse than the proposed algorithm which
// would require more implementation for about the same speed
Sphere::Sphere(Vector3f* points, uint32 amt)
{
	if(amt == 0) {
		data = VectorConstants::ZERO;
		return;
	}
	Vector3f extents[2];
	extents[0] = points[0];
	extents[1] = points[0];
	Vector3f meanPoint = VectorConstants::ZERO;
	for(uint32 i = 1; i < amt; i++) {
		Vector3f point = points[i];
		extents[0] = extents[0].min(point);
		extents[1] = extents[1].max(point);
		meanPoint += point;
	}
	Vector3f center1 = (extents[0]+extents[1])/2.0f;
	Vector3f center2 = meanPoint * Math::reciprocal(((float)amt));
	float radius21 = 0.0f;
	float radius22 = 0.0f;
	for(uint32 i = 0; i < amt; i++) {
		radius21 = Math::max(radius21, (center1-points[i]).lengthSquared());
		radius22 = Math::max(radius22, (center2-points[i]).lengthSquared());
	}
	if(radius22 < radius21) {
		data = center2.toVector(Math::sqrt(radius22));
	} else {
		data = center1.toVector(Math::sqrt(radius21));
	}
}

Sphere::Sphere(float* points, uint32 amt)
{
	if(amt == 0) {
		data = VectorConstants::ZERO;
		return;
	}
	Vector3f extents[2];
	Vector3f point0(points[0],points[1],points[2]);
	extents[0] = point0;
	extents[1] = point0;
	Vector3f meanPoint = VectorConstants::ZERO;
	uintptr index = 3;
	for(uint32 i = 1; i < amt; i++) {
		Vector3f point = Vector3f(points[index],points[index+1],points[index+2]);
		extents[0] = extents[0].min(point);
		extents[1] = extents[1].max(point);
		meanPoint += point;
		index += 3;
	}
	Vector3f center1 = (extents[0]+extents[1])/2.0f;
	Vector3f center2 = meanPoint * Math::reciprocal(((float)amt));
	float radius21 = 0.0f;
	float radius22 = 0.0f;
	index = 0;
	for(uint32 i = 0; i < amt; i++) {
		Vector3f point = Vector3f(points[index],points[index+1],points[index+2]);
		radius21 = Math::max(radius21, (center1-point).lengthSquared());
		radius22 = Math::max(radius22, (center2-point).lengthSquared());
		index += 3;
	}
	if(radius22 < radius21) {
		data = center2.toVector(Math::sqrt(radius22));
	} else {
		data = center1.toVector(Math::sqrt(radius21));
	}

}

Sphere Sphere::transform(const Matrix& transform) const
{
	Vector center = transform.transform(data.select(VectorConstants::MASK_W, VectorConstants::ONE));
	
	float radius = data[3]*Math::sqrt(Math::max3(
			transform[0].dot3(transform[0])[0],
			transform[1].dot3(transform[1])[0],
			transform[2].dot3(transform[2])[0]));

	return Sphere(Vector3f(center),radius);
}

Sphere Sphere::addSphere(const Sphere& other) const
{
	if(contains(other)) {
		return Sphere(*this);
	} else if(data[3] == 0.0f || other.contains(*this)) {
		return Sphere(other);
	} else {
		Vector3f dir = Vector3f(other.data - data);
		float dist = dir.length();
		dir /= dist;
		Vector3f extreme1 = getCenter() - dir*data[3];
		Vector3f extreme2 = other.getCenter() + dir*other.data[3];
		return Sphere(
				(extreme1+extreme2)*0.5f,
				(dist + data[3]+other.data[3])*0.5f);
	}
}

bool Sphere::intersectRay(const Vector3f& start, const Vector3f& rayDir, float& point1, float& point2) const
{
	Vector3f rayToCenter(getCenter() - start);
	float distToCenterAlongRay = rayToCenter.dot(rayDir);
	float sphereCenterDistToRaySq = rayToCenter.lengthSquared() - 
		distToCenterAlongRay*distToCenterAlongRay;
	float radius2 = data[3]*data[3];
	if(sphereCenterDistToRaySq > radius2) {
		return false;
	}
	float halfRayPenetrationDist = Math::sqrt(radius2-sphereCenterDistToRaySq);
	point1 = distToCenterAlongRay - halfRayPenetrationDist;
	point2 = distToCenterAlongRay + halfRayPenetrationDist;
	return true;
}

bool Sphere::intersectLine(const Vector3f& start, const Vector3f& end) const
{
	float p1, p2;
	Vector3f dir = (end-start);
	bool intersect = intersectRay(start, dir.normalized(), p1, p2);
	return intersect && p1*p1 < dir.lengthSquared();
}
