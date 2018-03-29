#include "plane.hpp"
#include "matrix.hpp"

Plane Plane::transform(const Matrix& transform) const
{
	return Plane(transform.toNormalMatrix().transform(data)).normalized();
}

bool Plane::intersectPlanes(Vector3f& intersectionPoint, const Plane& other1, const Plane& other2, float errorMargin) const
{
	Vector cross01 = data.cross3(other1.data);
	float det = cross01.dot3(other2.data)[0];
	float detSq = det*det;
	if(detSq < errorMargin) {
		return false;
	}
	Vector cross12 = other1.data.cross3(other2.data);
	Vector cross20 = other2.data.cross3(data);
	Vector w0 = data.replicate(3);
	Vector w1 = other1.data.replicate(3);
	Vector w2 = other2.data.replicate(3);
	Vector invDet = Vector::load1f(-Math::reciprocal(det));

	intersectionPoint = Vector3f(invDet*(w0*cross12+w1*cross20+w2*cross01));
	return true;
}


