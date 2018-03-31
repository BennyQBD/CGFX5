#pragma once

#include "plane.hpp"
#include "aabb.hpp"

namespace Intersects
{
	static FORCEINLINE void intersectPlaneAABBFast(const Vector& center, const Vector& extents, 
			const Plane& plane, const Plane& absPlane, bool& isFullyInside, bool& isPartiallyInside)
	{
		float d = plane.dotVector(center);
		float r = absPlane.dotVector(extents);
		isFullyInside = d-r >= 0;
		isPartiallyInside = d+r > 0;
	}

	static FORCEINLINE bool intersectPlaneAABB(const AABB& aabb, const Plane& plane, bool& isFullyInside, bool& isPartiallyInside)
	{
		intersectPlaneAABBFast(aabb.getCenter().toVector(1.0f), aabb.getExtents().toVector(1.0f), plane, plane.abs(),
				isFullyInside, isPartiallyInside);
		return isPartiallyInside && !isFullyInside;
	}

	static FORCEINLINE bool intersectPlaneAABB(const AABB& aabb, const Plane& plane)
	{
		bool isFullyInside;
		bool isPartiallyInside;
		return intersectPlaneAABB(aabb, plane, isFullyInside, isPartiallyInside);
	}

	static FORCEINLINE void intersectPlaneSphereFast(const Vector& center, float radius, const Plane& plane, bool& isFullyInside, bool& isPartiallyInside)
	{
		float d = plane.dot(center);
		isFullyInside = d >= radius;
		isPartiallyInside = d >= -radius;
	}

	static FORCEINLINE bool intersectPlaneSphere(const Sphere& sphere, const Plane& plane, bool& isFullyInside, bool& isPartiallyInside)
	{
		intersectPlaneSphereFast(sphere.getCenter().toVector(1.0f), sphere.getRadius(), plane, isFullyInside, isPartiallyInside);
		return isPartiallyInside && !isFullyInside; 
	}

	static FORCEINLINE bool intersectPlaneSphere(const Sphere& sphere, const Plane& plane)
	{
		bool isFullyInside;
		bool isPartiallyInside;
		return intersectPlaneSphere(sphere, plane, isFullyInside, isPartiallyInside); 
	}

	static FORCEINLINE bool intersectSphereAABBFast(const Vector& sphereCenter, const Vector& aabbMins, const Vector& aabbMaxs, float radiusSq)
	{
		Vector result =
			  (sphereCenter - aabbMins).select(sphereCenter < aabbMins, VectorConstants::ZERO) 
			+ (sphereCenter - aabbMaxs).select(sphereCenter > aabbMaxs, VectorConstants::ZERO);
		return result.dot3(result)[0] < radiusSq;
	}


	static FORCEINLINE bool intersectSphereAABB(const Sphere& sphere, const AABB& aabb)
	{
		Vector sphereCenter = sphere.getCenter().toVector();
		float radiusSq = sphere.getRadius()*sphere.getRadius();
		Vector aabbMins = aabb.getMinExtents().toVector();
		Vector aabbMaxs = aabb.getMaxExtents().toVector();
		return intersectSphereAABBFast(sphereCenter, aabbMins, aabbMaxs, radiusSq);
	}
}
