#pragma once

#include "vector.hpp"
#include "quaternion.hpp"
#include "matrix.hpp"

class Transform
{
public:
	inline Transform() :
		translation(0.0f, 0.0f, 0.0f),
		rotation(0.0f, 0.0f, 0.0f, 1.0f),
		scale(1.0f, 1.0f, 1.0f) {}

	inline Transform(const Vector3f& translationIn) :
		translation(translationIn),
		rotation(0.0f, 0.0f, 0.0f, 1.0f),
		scale(1.0f, 1.0f, 1.0f) {}

	inline Transform(const Quaternion& rotationIn) :
		translation(0.0f, 0.0f, 0.0f),
		rotation(rotationIn),
		scale(1.0f, 1.0f, 1.0f) {}

	inline Transform(const Vector3f& translationIn, const Quaternion& rotationIn,
			const Vector3f& scaleIn) :
		translation(translationIn),
		rotation(rotationIn),
		scale(scaleIn) {}

	inline Vector transform(const Vector& vector) const;
	inline Vector transform(const Vector3f& vector, float w) const;
	inline Vector inverseTransform(const Vector& vector) const;
	inline Vector inverseTransform(const Vector3f& vector, float w) const;
	inline Matrix toMatrix() const;
	Matrix inverse() const;
	inline void normalizeRotation();
	inline bool isRotationNormalized();

	inline Transform operator+(const Transform& other) const;
	inline Transform operator+=(const Transform& other);
	inline Transform operator*(const Transform& other) const;
	inline Transform operator*=(const Transform& other);
	inline Transform operator*(float other) const;
	inline Transform operator*=(float other);

	inline Vector3f getTranslation() const;
	inline Quaternion getRotation() const;
	inline Vector3f getScale() const;
	inline void set(const Vector3f& translation,
			const Quaternion& rotation, const Vector3f& scale);
	inline void setTranslation(const Vector3f& translation);
	inline void setRotation(const Quaternion& rotation);
	inline void setScale(const Vector3f& scale);
private:
	Vector3f translation;
	Quaternion rotation;
	Vector3f scale;
};

inline Matrix Transform::toMatrix() const
{
	return Matrix::transformMatrix(translation, rotation, scale);
}

inline void Transform::normalizeRotation()
{
	rotation = rotation.normalized();
}

inline bool Transform::isRotationNormalized()
{
	return rotation.isNormalized();
}

inline Transform Transform::operator+(const Transform& other) const
{
	return Transform(translation + other.translation,
			rotation + other.rotation, scale + other.scale);
}

inline Transform Transform::operator+=(const Transform& other)
{
	translation += other.translation;
	rotation += other.rotation;
	scale += other.scale;
	return *this;
}

inline Transform Transform::operator*(const Transform& other) const
{
	return Transform(translation * other.translation,
			rotation * other.rotation, scale * other.scale);
}

inline Transform Transform::operator*=(const Transform& other)
{
	translation *= other.translation;
	rotation *= other.rotation;
	scale *= other.scale;
	return *this;
}

inline Transform Transform::operator*(float other) const
{
	return Transform(translation * other,
			rotation * other, scale * other);
}

inline Transform Transform::operator*=(float other)
{
	translation *= other;
	rotation *= other;
	scale *= other;
	return *this;
}

inline Vector Transform::transform(const Vector3f& vector, float w) const
{
	return (rotation.rotate(scale * vector) + translation * w).toVector(0.0f);
}

inline Vector Transform::transform(const Vector& vector) const
{
	return transform(Vector3f(vector), vector[3]);
}

inline Vector Transform::inverseTransform(const Vector3f& vector, float w) const
{
	return (rotation.inverse().rotate(vector - translation * w) * scale.reciprocal()).toVector(0.0f);
}

inline Vector Transform::inverseTransform(const Vector& vector) const
{
	return inverseTransform(Vector3f(vector), vector[3]);
}


inline Vector3f Transform::getTranslation() const
{
	return translation;
}

inline Quaternion Transform::getRotation() const
{
	return rotation;
}

inline Vector3f Transform::getScale() const
{
	return scale;
}

inline void Transform::set(const Vector3f& translationIn,
		const Quaternion& rotationIn, const Vector3f& scaleIn)
{
	translation = translationIn;
	rotation = rotationIn;
	scale = scaleIn;
}

inline void Transform::setTranslation(const Vector3f& val)
{
	translation = val;
}

inline void Transform::setRotation(const Quaternion& val)
{
	rotation = val;
}

inline void Transform::setScale(const Vector3f& val)
{
	scale = val;
}

