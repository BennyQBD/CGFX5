#pragma once

#include "vecmath.hpp"
#include "vector.hpp"
#include "quaternion.hpp"
#include "plane.hpp"

class Matrix
{
public:
	FORCEINLINE Matrix();
	FORCEINLINE Matrix(const Vector& vecX, const Vector& vecY,
			const Vector& vecZ, const Vector& vecOffset);

	static FORCEINLINE Matrix identity();
	static FORCEINLINE Matrix translate(const Vector3f& amt);
	static FORCEINLINE Matrix scale(const Vector3f& amt);
	static FORCEINLINE Matrix scale(float amt);
	static FORCEINLINE Matrix ortho(float left, float right,
			float bottom, float top, float near, float far);
	static FORCEINLINE Matrix perspective(float halfFov, float aspect,
			float nearZ, float farZ);
	static FORCEINLINE Matrix transformMatrix(const Vector3f& translation,
			const Quaternion& rotation, const Vector3f& scale);

	void extractFrustumPlanes(Plane* planes) const;
	Matrix toNormalMatrix() const;
	
	FORCEINLINE Matrix operator+ (const Matrix& other) const;
	FORCEINLINE Matrix& operator+= (const Matrix& other);
	FORCEINLINE Matrix operator* (const Matrix& other) const;
	FORCEINLINE Matrix& operator*= (const Matrix& other);
	FORCEINLINE Matrix operator* (float amt) const;
	FORCEINLINE Matrix& operator*= (float amt);
	FORCEINLINE bool operator==(const Matrix& other) const;
	FORCEINLINE bool operator!=(const Matrix& other) const;

	FORCEINLINE bool equals(const Matrix& other, float errorMargin=1.e-4f) const;
	FORCEINLINE Vector transform(const Vector& vector) const;

	FORCEINLINE Matrix transpose() const;
	FORCEINLINE float determinant4x4() const;
	FORCEINLINE float determinant3x3() const;
	FORCEINLINE Matrix inverse() const;

	FORCEINLINE Matrix applyScale(const Vector& scale);
	FORCEINLINE Vector removeScale(float errorMargin=1.e-8f);
	FORCEINLINE Vector getScale() const;
	Quaternion getRotation() const;
	FORCEINLINE Vector getTranslation() const;

	FORCEINLINE Vector operator[](uint32 index) const {
		assertCheck(index < 4);
		return m[index];
	}
private:
	Vector m[4];
};

FORCEINLINE Matrix Matrix::identity()
{
	return Matrix(
			Vector::make(1.0f, 0.0f, 0.0f, 0.0f),
			Vector::make(0.0f, 1.0f, 0.0f, 0.0f),
			Vector::make(0.0f, 0.0f, 1.0f, 0.0f),
			Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::translate(const Vector3f& amt)
{
	return Matrix(
		Vector::make(1.0f, 0.0f, 0.0f, amt[0]),
		Vector::make(0.0f, 1.0f, 0.0f, amt[1]),
		Vector::make(0.0f, 0.0f, 1.0f, amt[2]),
		Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::scale(const Vector3f& amt)
{
	return Matrix(
			Vector::make(amt[0], 0.0f, 0.0f, 0.0f),
			Vector::make(0.0f, amt[1], 0.0f, 0.0f),
			Vector::make(0.0f, 0.0f, amt[2], 0.0f),
			Vector::make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::scale(float amt)
{
	return scale(Vector3f(amt));
}

FORCEINLINE Matrix Matrix::ortho(float left, float right,
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

FORCEINLINE Matrix Matrix::perspective(float halfFov, float aspect,
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

FORCEINLINE Matrix Matrix::transformMatrix(const Vector3f& translation,
			const Quaternion& rotation, const Vector3f& scale)
{
	Matrix result;
	Vector::createTransformMatrix(&result, translation.toVector(), rotation.toVector(), scale.toVector());
	return result;
}


FORCEINLINE Matrix::Matrix() {}

FORCEINLINE Matrix::Matrix(const Vector& vecX, const Vector& vecY,
			const Vector& vecZ, const Vector& vecOffset)
{
	m[0] = vecX;
	m[1] = vecY;
	m[2] = vecZ;
	m[3] = vecOffset;
}
	
FORCEINLINE Matrix Matrix::operator+ (const Matrix& other) const
{
	Matrix result;
	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = m[i] + other.m[i];
	}
	return result;
}

FORCEINLINE Matrix& Matrix::operator+= (const Matrix& other)
{
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] + other.m[i];
	}
	return *this;
}

FORCEINLINE Matrix Matrix::operator* (const Matrix& other) const
{
	Matrix result;
	Vector::matrixMul(&result, this, &other);
	return result;
}

FORCEINLINE Matrix& Matrix::operator*= (const Matrix& other)
{
	Vector::matrixMul(this, this, &other);
	return *this;
}

FORCEINLINE Matrix Matrix::operator* (float amt) const
{
	Matrix result;
	Vector vecAmt = Vector::load1f(amt);
	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = m[i] * vecAmt;
	}
	return result;
}

FORCEINLINE Matrix& Matrix::operator*= (float amt)
{
	Vector vecAmt = Vector::load1f(amt);
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * vecAmt;
	}
	return *this;
}

FORCEINLINE bool Matrix::operator==(const Matrix& other) const
{
	for(uint32 i = 0; i < 4; i++) {
		if(!(m[i] != other.m[i]).isZero4f()) {
			return false;
		}
	}
	return true;
}

FORCEINLINE bool Matrix::operator!=(const Matrix& other) const
{
	return !(*this == other);
}


FORCEINLINE bool Matrix::equals(const Matrix& other, float errorMargin) const
{
	for(uint32 i = 0; i < 4; i++) {
		if(!(m[i].notEquals(other.m[i], errorMargin)).isZero4f()) {
			return false;
		}
	}
	return true;

}

FORCEINLINE Vector Matrix::transform(const Vector& vector) const
{
	return vector.transform(this);
}


FORCEINLINE Matrix Matrix::transpose() const
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

FORCEINLINE float Matrix::determinant4x4() const
{
	return Vector::matrixDeterminant4x4(nullptr, nullptr, m);
}

FORCEINLINE float Matrix::determinant3x3() const
{
	return Vector::matrixDeterminant3x3Vector(m);
}

FORCEINLINE Matrix Matrix::inverse() const
{
	Matrix result;
	Vector::matrixInverse(&result, this);
	return result;
}


FORCEINLINE Vector Matrix::getScale() const
{
	Vector invScale = VectorConstants::ZERO;
	for(uint32 i = 0; i < 4; i++) {
		invScale = invScale + m[i]*m[i];
	}
	invScale = invScale.rsqrt().select(VectorConstants::MASK_W,VectorConstants::ONE);
	return invScale.reciprocal();
}

FORCEINLINE Vector Matrix::removeScale(float errorMargin)
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

FORCEINLINE Vector Matrix::getTranslation() const
{
	return Vector::make(m[0][3], m[1][3], m[2][3], m[3][3]);
}

FORCEINLINE Matrix Matrix::applyScale(const Vector& scale)
{
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * scale;//Vector::make(scale[i], scale[i], scale[i], 0.0f);
	}
	return *this;
}
