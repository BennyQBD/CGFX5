#include "vector.hpp"

bool Vector3f::operator==(const Vector3f& other) const
{
	return (vec != other.vec).isZero3f();
}

bool Vector3f::operator!=(const Vector3f& other) const
{
	return !(*this == other);
}

bool Vector3f::equals(const Vector3f& other, float errorMargin) const
{
	return vec.notEquals(other.vec, errorMargin).isZero3f();
}

bool Vector3f::equals(float val, float errorMargin) const
{
	return equals(Vector3f(val), errorMargin);
}


float Vector3f::operator[](uint32 index) const
{
	assertCheck(index < 3);
	return vec[index];
}


void Vector3f::set(float x, float y, float z)
{
	vec = Vector::make(x, y, z, 0.0f);
}

void Vector3f::set(uint32 index, float val)
{
	vec = vec.select(Vector::mask(index), Vector::load1f(val));
}

float Vector3f::max() const
{
	float vals[3];
	vec.store3f(vals);
	return Math::max3(vals[0], vals[1], vals[2]);
}

float Vector3f::min() const
{
	float vals[3];
	vec.store3f(vals);
	return Math::min3(vals[0], vals[1], vals[2]);
}

float Vector3f::absMax() const
{
	float vals[3];
	vec.abs().store3f(vals);
	return Math::max3(vals[0], vals[1], vals[2]);
}

float Vector3f::absMin() const
{
	float vals[3];
	vec.abs().store3f(vals);
	return Math::min3(vals[0], vals[1], vals[2]);
}


Vector3f Vector3f::abs() const
{
	return Vector3f(vec.abs());
}

Vector3f Vector3f::min(const Vector3f& other) const
{
	return Vector3f(vec.min(other.vec));
}

Vector3f Vector3f::max(const Vector3f& other) const
{
	return Vector3f(vec.max(other.vec));
}

Vector3f Vector3f::normalized(float errorMargin) const
{
	// Currently does not use errorMargin.
	(void)errorMargin;
	return Vector3f(vec.normalize3());
}

bool Vector3f::isNormalized(float errorMargin) const
{
	return Math::abs(1.0f - lengthSquared()) < errorMargin;
}


void Vector3f::dirAndLength(Vector3f& dir, float& length) const
{
	Vector rlen = vec.dot3(vec).rsqrt();
	dir = Vector3f(vec * rlen);
	length = Math::reciprocal(rlen[0]);
}

Vector3f Vector3f::project() const
{
	Vector rprojectVal = Vector::load1f(Math::reciprocal(vec[2]));
	return Vector3f(vec * rprojectVal);
}

Vector3f Vector3f::reciprocal() const
{
	return Vector3f(vec.reciprocal());
}


Vector3f Vector3f::rotate(const Vector3f& axis, float angle) const
{
	float sinAngle;
	float cosAngle;
	
	Math::sincos(&sinAngle, &cosAngle, -angle);
	Vector sinVec = Vector::load1f(sinAngle);
	Vector cosVec = Vector::load1f(cosAngle);
	Vector oneMinusCosVec = Vector::load1f(1.0f - cosAngle);
	
	Vector rotatedX = vec.cross3(axis.vec * sinVec);
	Vector rotatedY = axis.vec * vec.dot3(axis.vec * oneMinusCosVec);
	Vector rotatedZ = vec * cosVec;

	return Vector3f(rotatedX + rotatedY + rotatedZ);
}


Vector3f Vector3f::reflect(const Vector3f& normal) const
{
	Vector dotAmt = VectorConstants::TWO * vec.dot3(normal.vec);
	return Vector3f(vec - (normal.vec * dotAmt));
}

Vector3f Vector3f::refract(const Vector3f& normal, float indexOfRefraction) const
{
	float cosNormalAngle = vec.dot3(normal.vec)[0];
	float refractanceSquared = 
		 1.0f - indexOfRefraction * indexOfRefraction *
		(1.0f - cosNormalAngle * cosNormalAngle);
	
	if(refractanceSquared < 0.0f) {
		return Vector3f(VectorConstants::ZERO);
	}

	float normalScale = indexOfRefraction * cosNormalAngle + Math::sqrt(refractanceSquared);
	Vector normalScaleVec(Vector::load1f(normalScale));
	Vector indexOfRefractionVec(Vector::load1f(indexOfRefraction));
	
	return Vector3f(vec * indexOfRefractionVec - normalScaleVec * normal.vec);
}

Vector3f Vector3f::toDegrees() const
{
	return Vector3f(vec * Vector::load1f(Math::RAD_TO_DEG_CONV));
}

Vector3f Vector3f::toRadians() const
{
	return Vector3f(vec * Vector::load1f(Math::DEG_TO_RAD_CONV));
}

Vector Vector3f::toVector(float w) const
{
	return vec.select(VectorConstants::MASK_W, Vector::load1f(w));
}

