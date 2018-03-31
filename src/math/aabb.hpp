#pragma once

#include "vector.hpp"
#include "matrix.hpp"

class AABB
{
public:
	inline AABB() {}
	inline AABB(const Vector3f& minExtents, const Vector3f& maxExtents);
	AABB(Vector3f* points, uint32 amt);
	AABB(float* points, uint32 amt, uint32 stride=0);

	bool intersectRay(const Vector3f& start, const Vector3f& rayDir, float& point1, float& point2) const;
	bool intersectLine(const Vector3f& start, const Vector3f& end) const;
	inline bool intersects(const AABB& other) const;
	AABB transform(const Matrix& transform) const;

	inline AABB expand(float distance) const;
	inline AABB expand(const Vector3f& amt) const;
	inline AABB moveTo(const Vector3f& destination) const;
	inline Vector3f getCenter() const;
	inline Vector3f getExtents() const;
	inline Vector3f getMinExtents() const;
	inline Vector3f getMaxExtents() const;
	inline void getCenterAndExtents(Vector3f& center, Vector3f& extents) const;
	inline float getVolume() const;
	inline AABB overlap(const AABB& other) const;
	inline bool contains(const Vector3f& point) const;
	inline bool contains(const AABB& other) const;

	inline AABB translate(const Vector3f& amt) const;
	inline AABB scaleFromCenter(const Vector3f& amt) const;
	inline AABB scaleFromOrigin(const Vector3f& amt) const;

	inline AABB addPoint(const Vector3f& other) const;
	inline AABB addAABB(const AABB& other) const;

	inline bool operator==(const AABB& other) const;
	inline bool operator!=(const AABB& other) const;
	inline bool equals(const AABB& other, float errorMargin=1.e-4f) const;
private:
	Vector3f extents[2];
};


inline AABB::AABB(const Vector3f& minExtents, const Vector3f& maxExtents)
{
	extents[0] = minExtents;
	extents[1] = maxExtents;
}

inline bool AABB::intersects(const AABB& other) const
{
	return
		((extents[0].toVector() >= other.extents[1].toVector()) |
		 (extents[1].toVector() <= other.extents[0].toVector())).isZero3f();
}

inline AABB AABB::expand(float distance) const
{
	return expand(Vector3f(distance));
}

inline AABB AABB::expand(const Vector3f& amt) const
{
	return AABB(extents[0]-amt, extents[1]+amt);
}

inline AABB AABB::moveTo(const Vector3f& destination) const
{
	return translate(destination-getCenter());
}

inline Vector3f AABB::getCenter() const
{
	return (extents[1]+extents[0]) * 0.5f;
}

inline Vector3f AABB::getExtents() const
{
	return (extents[1]-extents[0]) * 0.5f;
}

inline Vector3f AABB::getMinExtents() const
{
	return extents[0];
}

inline Vector3f AABB::getMaxExtents() const
{
	return extents[1];
}

inline void AABB::getCenterAndExtents(Vector3f& center, Vector3f& extents) const
{
	extents = (this->extents[1]-this->extents[0]) * 0.5f;
	center = this->extents[0]+extents;
}

inline float AABB::getVolume() const
{
	Vector3f lengths = extents[1]-extents[0];
	return lengths[0]*lengths[1]*lengths[2];
}

inline AABB AABB::overlap(const AABB& other) const
{
	return AABB(extents[0].max(other.extents[0]),
			extents[1].min(other.extents[1]));
}

inline bool AABB::contains(const Vector3f& point) const
{
	return ((point.toVector() <= extents[0].toVector()) |
	 (point.toVector() >= extents[1].toVector())).isZero3f();
}

inline bool AABB::contains(const AABB& other) const
{
	return
		((other.extents[0].toVector() <= extents[0].toVector()) |
	 	(other.extents[0].toVector() >= extents[1].toVector()) |
		(other.extents[1].toVector() <= extents[0].toVector()) |
	 	(other.extents[1].toVector() >= extents[1].toVector())).isZero3f();
}

inline AABB AABB::addPoint(const Vector3f& other) const
{
	return AABB(extents[0].min(other), extents[1].max(other));
}

inline AABB AABB::addAABB(const AABB& other) const
{
	return AABB(extents[0].min(other.extents[0]), extents[1].max(other.extents[1]));
}

inline AABB AABB::translate(const Vector3f& amt) const
{
	return AABB(extents[0]+amt, extents[1]+amt);
}

inline AABB AABB::scaleFromOrigin(const Vector3f& amt) const
{
	return AABB(extents[0]*amt, extents[1]*amt);
}

inline AABB AABB::scaleFromCenter(const Vector3f& amt) const
{
	Vector3f extents, center;
	getCenterAndExtents(center, extents);
	extents = extents * amt;
	return AABB(center-extents, center+extents);
}

inline bool AABB::operator==(const AABB& other) const
{
	return (extents[0] == other.extents[0]) && (extents[1] == other.extents[1]);
}

inline bool AABB::operator!=(const AABB& other) const
{
	return (extents[0] != other.extents[0]) && (extents[1] != other.extents[1]);
}

inline bool AABB::equals(const AABB& other, float errorMargin) const
{
	return
		extents[0].equals(other.extents[0], errorMargin) && 
		extents[1].equals(other.extents[1], errorMargin);
}

