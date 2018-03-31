#pragma once

#include "vector.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

class Transform
{
public:
	FORCEINLINE Transform() :
		translation(0.0f, 0.0f, 0.0f),
		rotation(0.0f, 0.0f, 0.0f, 1.0f),
		scale(1.0f, 1.0f, 1.0f) {}

	FORCEINLINE Transform(const Vector3f& translationIn) :
		translation(translationIn),
		rotation(0.0f, 0.0f, 0.0f, 1.0f),
		scale(1.0f, 1.0f, 1.0f) {}

	FORCEINLINE Transform(const Quaternion& rotationIn) :
		translation(0.0f, 0.0f, 0.0f),
		rotation(rotationIn),
		scale(1.0f, 1.0f, 1.0f) {}

	FORCEINLINE Transform(const Vector3f& translationIn, const Quaternion& rotationIn,
			const Vector3f& scaleIn) :
		translation(translationIn),
		rotation(rotationIn),
		scale(scaleIn) {}

	FORCEINLINE Vector transform(const Vector& vector) const;
	FORCEINLINE Vector transform(const Vector3f& vector, float w) const;
	FORCEINLINE Vector inverseTransform(const Vector& vector) const;
	FORCEINLINE Vector inverseTransform(const Vector3f& vector, float w) const;
	FORCEINLINE Matrix toMatrix() const;
	Matrix inverse() const;
	FORCEINLINE void normalizeRotation();
	FORCEINLINE bool isRotationNormalized();

	FORCEINLINE Transform operator+(const Transform& other) const;
	FORCEINLINE Transform operator+=(const Transform& other);
	FORCEINLINE Transform operator*(const Transform& other) const;
	FORCEINLINE Transform operator*=(const Transform& other);
	FORCEINLINE Transform operator*(float other) const;
	FORCEINLINE Transform operator*=(float other);

	FORCEINLINE Vector3f getTranslation() const;
	FORCEINLINE Quaternion getRotation() const;
	FORCEINLINE Vector3f getScale() const;
	FORCEINLINE void set(const Vector3f& translation,
			const Quaternion& rotation, const Vector3f& scale);
	FORCEINLINE void setTranslation(const Vector3f& translation);
	FORCEINLINE void setRotation(const Quaternion& rotation);
	FORCEINLINE void setScale(const Vector3f& scale);
private:
	Vector3f translation;
	Quaternion rotation;
	Vector3f scale;
};

FORCEINLINE Matrix Transform::toMatrix() const
{
	return Matrix::transformMatrix(translation, rotation, scale);
}

FORCEINLINE void Transform::normalizeRotation()
{
	rotation = rotation.normalized();
}

FORCEINLINE bool Transform::isRotationNormalized()
{
	return rotation.isNormalized();
}

FORCEINLINE Transform Transform::operator+(const Transform& other) const
{
	return Transform(translation + other.translation,
			rotation + other.rotation, scale + other.scale);
}

FORCEINLINE Transform Transform::operator+=(const Transform& other)
{
	translation += other.translation;
	rotation += other.rotation;
	scale += other.scale;
	return *this;
}

FORCEINLINE Transform Transform::operator*(const Transform& other) const
{
	return Transform(translation * other.translation,
			rotation * other.rotation, scale * other.scale);
}

FORCEINLINE Transform Transform::operator*=(const Transform& other)
{
	translation *= other.translation;
	rotation *= other.rotation;
	scale *= other.scale;
	return *this;
}

FORCEINLINE Transform Transform::operator*(float other) const
{
	return Transform(translation * other,
			rotation * other, scale * other);
}

FORCEINLINE Transform Transform::operator*=(float other)
{
	translation *= other;
	rotation *= other;
	scale *= other;
	return *this;
}

FORCEINLINE Vector Transform::transform(const Vector3f& vector, float w) const
{
	return (rotation.rotate(scale * vector) + translation * w).toVector(0.0f);
}

FORCEINLINE Vector Transform::transform(const Vector& vector) const
{
	return transform(Vector3f(vector), vector[3]);
}

FORCEINLINE Vector Transform::inverseTransform(const Vector3f& vector, float w) const
{
	return (rotation.inverse().rotate(vector - translation * w) * scale.reciprocal()).toVector(0.0f);
}

FORCEINLINE Vector Transform::inverseTransform(const Vector& vector) const
{
	return inverseTransform(Vector3f(vector), vector[3]);
}


FORCEINLINE Vector3f Transform::getTranslation() const
{
	return translation;
}

FORCEINLINE Quaternion Transform::getRotation() const
{
	return rotation;
}

FORCEINLINE Vector3f Transform::getScale() const
{
	return scale;
}

FORCEINLINE void Transform::set(const Vector3f& translationIn,
		const Quaternion& rotationIn, const Vector3f& scaleIn)
{
	translation = translationIn;
	rotation = rotationIn;
	scale = scaleIn;
}

FORCEINLINE void Transform::setTranslation(const Vector3f& val)
{
	translation = val;
}

FORCEINLINE void Transform::setRotation(const Quaternion& val)
{
	rotation = val;
}

FORCEINLINE void Transform::setScale(const Vector3f& val)
{
	scale = val;
}

