#include "quaternion.hpp"

Quaternion Quaternion::normalized(float errorMargin) const
{
	static const Vector defaultQuat = Vector::make(0.0f, 0.0f, 0.0f, 1.0f);
	Vector lenSq = vec.dot4(vec);
	Vector mask = lenSq >= Vector::load1f(errorMargin);
	Vector normalizedVec = vec * lenSq.rsqrt();
	return Quaternion(normalizedVec.select(mask, defaultQuat));
}

bool Quaternion::isNormalized(float errorMargin) const
{
	return Math::abs(1.0f - lengthSquared()) < errorMargin;
}


Vector3f Quaternion::getAxis() const
{
	float w = vec[3];
	float rangleDivisor = Math::rsqrt(Math::max(1.0f - w*w, 0.0f));
	return Vector3f(vec * Vector::load1f(rangleDivisor));
}

float Quaternion::getAngle() const
{
	return 2.0f * Math::acos(vec[3]);
}

void Quaternion::axisAndAngle(Vector3f& axis, float& angle) const
{
	angle = getAngle();
	axis = getAxis();
}


Vector3f Quaternion::rotate(const Vector3f& other) const
{
	return Vector3f(vec.quatRotateVec(other.toVector()));
}

Quaternion Quaternion::slerp(const Quaternion& dest, float amt, float errorMargin) const
{
	float cosAngleInitial = dot(dest);
	float cosAngle = Math::select(cosAngleInitial, cosAngleInitial, -cosAngleInitial);

	float lerpAmt1 = 1.0f - amt;
	float lerpAmt2 = amt;
	if(cosAngle < (1.0f - errorMargin))
	{
		float rsinAngle = Math::rsqrt(1.0f - cosAngle * cosAngle);
		float angle = Math::acos(cosAngle);
		// NOTE: You can also get rsinangle from doing
		//     Math::reciprocal(Math::sin(angle));
		lerpAmt1 = Math::sin(lerpAmt1 * angle) * rsinAngle;
		lerpAmt2 = Math::sin(lerpAmt2 * angle) * rsinAngle;
	}

	lerpAmt2 = Math::select(cosAngleInitial, lerpAmt2, -lerpAmt2);

	Vector lerpAmt1Vec = Vector::load1f(lerpAmt1);
	Vector lerpAmt2Vec = Vector::load1f(lerpAmt2);
	return Quaternion(vec * lerpAmt1Vec + dest.vec * lerpAmt2Vec);
}

Quaternion Quaternion::conjugate() const
{
	static const Vector inverter = Vector::make(-1.0f, -1.0f, -1.0f, 1.0f);
	return Quaternion(vec * inverter);
}

Quaternion Quaternion::inverse() const
{
	return normalized().conjugate();
}

