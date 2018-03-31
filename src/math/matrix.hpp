#pragma once

#include "vecmath.hpp"
#include "vector.hpp"
#include "quaternion.hpp"
#include "plane.hpp"

class Matrix
{
public:
	inline Matrix();
	inline Matrix(const Vector& vecX, const Vector& vecY,
			const Vector& vecZ, const Vector& vecOffset);

	static inline Matrix identity();
	static inline Matrix translate(const Vector3f& amt);
	static inline Matrix scale(const Vector3f& amt);
	static inline Matrix scale(float amt);
	static inline Matrix ortho(float left, float right,
			float bottom, float top, float near, float far);
	static inline Matrix perspective(float halfFov, float aspect,
			float nearZ, float farZ);
	static inline Matrix transformMatrix(const Vector3f& translation,
			const Quaternion& rotation, const Vector3f& scale);

	void extractFrustumPlanes(Plane* planes) const;
	Matrix toNormalMatrix() const;
	
	inline Matrix operator+ (const Matrix& other) const;
	inline Matrix& operator+= (const Matrix& other);
	inline Matrix operator* (const Matrix& other) const;
	inline Matrix& operator*= (const Matrix& other);
	inline Matrix operator* (float amt) const;
	inline Matrix& operator*= (float amt);
	inline bool operator==(const Matrix& other) const;
	inline bool operator!=(const Matrix& other) const;

	inline bool equals(const Matrix& other, float errorMargin=1.e-4f) const;
	inline Vector transform(const Vector& vector) const;

	inline Matrix transpose() const;
	inline float determinant4x4() const;
	inline float determinant3x3() const;
	inline Matrix inverse() const;

	inline Matrix applyScale(const Vector& scale);
	inline Vector removeScale(float errorMargin=1.e-8f);
	inline Vector getScale() const;
	Quaternion getRotation() const;
	inline Vector getTranslation() const;

	inline Vector operator[](uint32 index) const {
		assertCheck(index < 4);
		return m[index];
	}
private:
	Vector m[4];
};

inline Matrix Matrix::identity()
{
	return Matrix(
			Vector::make(1.0f, 0.0f, 0.0f, 0.0f),
			Vector::make(0.0f, 1.0f, 0.0f, 0.0f),
			Vector::make(0.0f, 0.0f, 1.0f, 0.0f),
			Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Matrix Matrix::translate(const Vector3f& amt)
{
	return Matrix(
		Vector::make(1.0f, 0.0f, 0.0f, amt[0]),
		Vector::make(0.0f, 1.0f, 0.0f, amt[1]),
		Vector::make(0.0f, 0.0f, 1.0f, amt[2]),
		Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Matrix Matrix::scale(const Vector3f& amt)
{
	return Matrix(
			Vector::make(amt[0], 0.0f, 0.0f, 0.0f),
			Vector::make(0.0f, amt[1], 0.0f, 0.0f),
			Vector::make(0.0f, 0.0f, amt[2], 0.0f),
			Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Matrix Matrix::scale(float amt)
{
	return scale(Vector3f(amt));
}

inline Matrix Matrix::ortho(float left, float right,
			float bottom, float top, float near, float far)
{
	float rwidth = Math::reciprocal(right - left);
	float rheight = Math::reciprocal(top - bottom);
	float rdepth = Math::reciprocal(far - near);

	return Matrix(
			Vector::make(2.0f * rwidth, 0.0f, 0.0f, -(right + left) * rwidth),
			Vector::make(0.0f, 2.0f * rheight, 0.0f, -(top + bottom) * rheight),
			Vector::make(0.0f, 0.0f, 2.0f * rdepth, -(far + near) * rdepth),
			Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

inline Matrix Matrix::perspective(float halfFov, float aspect,
			float nearZ, float farZ)
{
	float rtanHalfFov = Math::reciprocal(Math::tan(halfFov));
	float scaleZ = nearZ == farZ ? 1.0f : farZ / (farZ - nearZ);
	float offsetZ = -nearZ * scaleZ;

	return Matrix(
			Vector::make(rtanHalfFov, 0.0f, 0.0f, 0.0f),
			Vector::make(0.0f, aspect * rtanHalfFov, 0.0f, 0.0f),
			Vector::make(0.0f, 0.0f, scaleZ, offsetZ),
			Vector::make(0.0f, 0.0f, 1.0f, 1.0f));
}

inline Matrix Matrix::transformMatrix(const Vector3f& translation,
			const Quaternion& rotation, const Vector3f& scale)
{
	Matrix result;
	Vector::createTransformMatrix(&result, translation.toVector(), rotation.toVector(), scale.toVector());
	return result;
}


inline Matrix::Matrix() {}

inline Matrix::Matrix(const Vector& vecX, const Vector& vecY,
			const Vector& vecZ, const Vector& vecOffset)
{
	m[0] = vecX;
	m[1] = vecY;
	m[2] = vecZ;
	m[3] = vecOffset;
}
	
inline Matrix Matrix::operator+ (const Matrix& other) const
{
	Matrix result;
	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = m[i] + other.m[i];
	}
	return result;
}

inline Matrix& Matrix::operator+= (const Matrix& other)
{
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] + other.m[i];
	}
	return *this;
}

inline Matrix Matrix::operator* (const Matrix& other) const
{
	Matrix result;
	Vector::matrixMul(&result, this, &other);
	return result;
}

inline Matrix& Matrix::operator*= (const Matrix& other)
{
	Vector::matrixMul(this, this, &other);
	return *this;
}

inline Matrix Matrix::operator* (float amt) const
{
	Matrix result;
	Vector vecAmt = Vector::load1f(amt);
	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = m[i] * vecAmt;
	}
	return result;
}

inline Matrix& Matrix::operator*= (float amt)
{
	Vector vecAmt = Vector::load1f(amt);
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * vecAmt;
	}
	return *this;
}

inline bool Matrix::operator==(const Matrix& other) const
{
	for(uint32 i = 0; i < 4; i++) {
		if(!(m[i] != other.m[i]).isZero4f()) {
			return false;
		}
	}
	return true;
}

inline bool Matrix::operator!=(const Matrix& other) const
{
	return !(*this == other);
}


inline bool Matrix::equals(const Matrix& other, float errorMargin) const
{
	for(uint32 i = 0; i < 4; i++) {
		if(!(m[i].notEquals(other.m[i], errorMargin)).isZero4f()) {
			return false;
		}
	}
	return true;

}

inline Vector Matrix::transform(const Vector& vector) const
{
	return vector.transform(this);
}


inline Matrix Matrix::transpose() const
{
	Matrix result;
	float m0[4];
	float m1[4];
	float m2[4];
	float m3[4];
	m[0].store4f(m0);
	m[1].store4f(m1);
	m[2].store4f(m2);
	m[3].store4f(m3);

	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = Vector::make(m0[i], m1[i], m2[i], m3[i]);
	}

	return result;
}

inline float Matrix::determinant4x4() const
{
	return Vector::matrixDeterminant4x4(nullptr, nullptr, m);
}

inline float Matrix::determinant3x3() const
{
	return Vector::matrixDeterminant3x3Vector(m);
}

inline Matrix Matrix::inverse() const
{
	Matrix result;
	Vector::matrixInverse(&result, this);
	return result;
}


inline Vector Matrix::getScale() const
{
	Vector invScale = VectorConstants::ZERO;
	for(uint32 i = 0; i < 4; i++) {
		invScale = invScale + m[i]*m[i];
	}
	invScale = invScale.rsqrt().select(VectorConstants::MASK_W,VectorConstants::ONE);
	return invScale.reciprocal();
}

inline Vector Matrix::removeScale(float errorMargin)
{
	Vector invScale = VectorConstants::ZERO;
	for(uint32 i = 0; i < 4; i++) {
		invScale = invScale + m[i]*m[i];
	}
	invScale = invScale.rsqrt().select(VectorConstants::MASK_W,VectorConstants::ONE);
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * invScale;
	}
	return invScale.reciprocal();
}

inline Vector Matrix::getTranslation() const
{
	return Vector::make(m[0][3], m[1][3], m[2][3], m[3][3]);
}

inline Matrix Matrix::applyScale(const Vector& scale)
{
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * scale;//Vector::make(scale[i], scale[i], scale[i], 0.0f);
	}
	return *this;
}
