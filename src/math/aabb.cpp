#include "aabb.hpp"

AABB::AABB(Vector3f* points, uint32 amt)
{
	if(amt == 0) {
		extents[0] = Vector3f(0.0f,0.0f,0.0f);
		extents[1] = Vector3f(0.0f,0.0f,0.0f);
		return;
	}
	extents[0] = points[0];
	extents[1] = points[0];
	for(uint32 i = 1; i < amt; i++) {
		extents[0] = extents[0].min(points[i]);
		extents[1] = extents[1].max(points[i]);
	}
}

AABB::AABB(float* points, uint32 amt, uint32 stride)
{
	if(amt == 0) {
		extents[0] = Vector3f(0.0f,0.0f,0.0f);
		extents[1] = Vector3f(0.0f,0.0f,0.0f);
		return;
	}
	Vector3f initialPoint(points[0],points[1],points[2]);
	extents[0] = initialPoint;
	extents[1] = initialPoint;
	uintptr index = 3;
	stride += 3;
	for(uint32 i = 1; i < amt; i++) {
		Vector3f point(points[index],points[index+1],points[index+2]);
		extents[0] = extents[0].min(point);
		extents[1] = extents[1].max(point);
		index += stride;
	}
}

AABB AABB::transform(const Matrix& transform) const
{
	Vector center(getCenter().toVector(1.0f));
	Vector extents(getExtents().toVector(0.0f));
	Vector absExtents = extents.abs();
	Matrix absMatrix(transform);
	for(uint32 i = 0; i < 4; i++) {
		absMatrix[i]=absMatrix[i].abs();
	}

	Vector newCenter = transform.transform(center);
	Vector newExtents = absMatrix.transform(absExtents);
	return AABB(newCenter-newExtents, newCenter+newExtents);
}

bool AABB::intersectRay(const Vector3f& start, const Vector3f& rayDir, float& point1, float& point2) const
{
	Vector startVec = start.toVector();
	Vector dirVec = rayDir.toVector();
	Vector minVec = extents[0].toVector();
	Vector maxVec = extents[1].toVector();
	Vector rdirVec = dirVec.reciprocal();
	Vector intersects1 = (minVec-startVec)*rdirVec;
	Vector intersects2 = (maxVec-startVec)*rdirVec;

	float mins[4];
	float maxs[4];
	intersects1.min(intersects2).store4f(mins);
	intersects1.max(intersects2).store4f(maxs);

	if((mins[0] > maxs[1]) || (mins[1] > maxs[0])) {
		return false;
	}
	if(mins[1] > mins[0]) {
		mins[0] = mins[1];
	}
	if(maxs[1] < maxs[0]) {
		maxs[0] = maxs[1];
	}
	if((mins[0] > maxs[2]) || (mins[2] > maxs[0])) {
		return false;
	}
	if(mins[2] > mins[0]) {
		mins[0] = mins[2];
	}
	if(maxs[2] < maxs[0]) {
		maxs[0] = maxs[2];
	}
	point1 = mins[0];
	point2 = maxs[0];
	return true;
}

bool AABB::intersectLine(const Vector3f& start, const Vector3f& end) const
{
	float p1, p2;
	Vector3f dir = (end-start);
	bool intersect = intersectRay(start, dir.normalized(), p1, p2);
	return intersect && p1*p1 < dir.lengthSquared();
} 
