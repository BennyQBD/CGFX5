#pragma once

#include "core/memory.hpp"
#include "math/math.hpp"

struct GenericVector
{
public:
	static FORCEINLINE void matrixMul(void* result, const void* mat1, const void* mat2)
	{
		const GenericVector* m1 = (const GenericVector*)mat1;
		const GenericVector* m2 = (const GenericVector*)mat2;
		GenericVector* r = (GenericVector*)result;
		GenericVector temp, r0, r1, r2, r3;

		temp = m1[0].replicate(0) * m2[0];
		temp = m1[0].replicate(1).mad(m2[1], temp);
		temp = m1[0].replicate(2).mad(m2[2], temp);
		r0 = m1[0].replicate(3).mad(m2[3], temp);

		temp = m1[1].replicate(0) * m2[0];
		temp = m1[1].replicate(1).mad(m2[1], temp);
		temp = m1[1].replicate(2).mad(m2[2], temp);
		r1 = m1[1].replicate(3).mad(m2[3], temp);

		temp = m1[2].replicate(0) * m2[0];
		temp = m1[2].replicate(1).mad(m2[1], temp);
		temp = m1[2].replicate(2).mad(m2[2], temp);
		r2 = m1[2].replicate(3).mad(m2[3], temp);

		temp = m1[3].replicate(0) * m2[0];
		temp = m1[3].replicate(1).mad(m2[1], temp);
		temp = m1[3].replicate(2).mad(m2[2], temp);
		r3 = m1[3].replicate(3).mad(m2[3], temp);

		r[0] = r0;
		r[1] = r1;
		r[2] = r2;
		r[3] = r3;
	}

	static FORCEINLINE float matrixDeterminant3x3Vector(const GenericVector* m)
	{
		float M[4][4];
		for(uint32 i = 0; i < 4; i++) {
			m[i].store4f(M[i]);
		}
		return 
			M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
			M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
			M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
	}

	static FORCEINLINE float matrixDeterminant4x4(float* outS,
			float* outC, const void* mat)
	{
		float sVals[6];
		float cVals[6];

		float* s = outS == nullptr ? sVals : outS;
		float* c = outC == nullptr ? cVals : outC;

		GenericVector* m = (GenericVector*)mat;
		float M[4][4];
		for(uint32 i = 0; i < 4; i++) {
			m[i].store4f(M[i]);
		}
		
		s[0] = M[0][0] * M[1][1] - M[1][0] * M[0][1];
		s[1] = M[0][0] * M[1][2] - M[1][0] * M[0][2];
		s[2] = M[0][0] * M[1][3] - M[1][0] * M[0][3];
		s[3] = M[0][1] * M[1][2] - M[1][1] * M[0][2];
		s[4] = M[0][1] * M[1][3] - M[1][1] * M[0][3];
		s[5] = M[0][2] * M[1][3] - M[1][2] * M[0][3];

		c[5] = M[2][2] * M[3][3] - M[3][2] * M[2][3];
		c[4] = M[2][1] * M[3][3] - M[3][1] * M[2][3];
		c[3] = M[2][1] * M[3][2] - M[3][1] * M[2][2];
		c[2] = M[2][0] * M[3][3] - M[3][0] * M[2][3];
		c[1] = M[2][0] * M[3][2] - M[3][0] * M[2][2];
		c[0] = M[2][0] * M[3][1] - M[3][0] * M[2][1];

		return (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);
	}

	static FORCEINLINE void matrixInverse(void* dest, const void* src)
	{
		float s[6];
		float c[6];
		float rdet = Math::reciprocal(matrixDeterminant4x4(s, c, src));

		GenericVector* m = (GenericVector*)src;
		float M[4][4];
		for(uint32 i = 0; i < 4; i++) {
			m[i].store4f(M[i]);
		}

		float result[4][4];
		result[0][0] = ( M[1][1] * c[5] - M[1][2] * c[4] + M[1][3] * c[3]) * rdet;
		result[0][1] = (-M[0][1] * c[5] + M[0][2] * c[4] - M[0][3] * c[3]) * rdet;
		result[0][2] = ( M[3][1] * s[5] - M[3][2] * s[4] + M[3][3] * s[3]) * rdet;
		result[0][3] = (-M[2][1] * s[5] + M[2][2] * s[4] - M[2][3] * s[3]) * rdet;

		result[1][0] = (-M[1][0] * c[5] + M[1][2] * c[2] - M[1][3] * c[1]) * rdet;
		result[1][1] = ( M[0][0] * c[5] - M[0][2] * c[2] + M[0][3] * c[1]) * rdet;
		result[1][2] = (-M[3][0] * s[5] + M[3][2] * s[2] - M[3][3] * s[1]) * rdet;
		result[1][3] = ( M[2][0] * s[5] - M[2][2] * s[2] + M[2][3] * s[1]) * rdet;

		result[2][0] = ( M[1][0] * c[4] - M[1][1] * c[2] + M[1][3] * c[0]) * rdet;
		result[2][1] = (-M[0][0] * c[4] + M[0][1] * c[2] - M[0][3] * c[0]) * rdet;
		result[2][2] = ( M[3][0] * s[4] - M[3][1] * s[2] + M[3][3] * s[0]) * rdet;
		result[2][3] = (-M[2][0] * s[4] + M[2][1] * s[2] - M[2][3] * s[0]) * rdet;

		result[3][0] = (-M[1][0] * c[3] + M[1][1] * c[1] - M[1][2] * c[0]) * rdet;
		result[3][1] = ( M[0][0] * c[3] - M[0][1] * c[1] + M[0][2] * c[0]) * rdet;
		result[3][2] = (-M[3][0] * s[3] + M[3][1] * s[1] - M[3][2] * s[0]) * rdet;
		result[3][3] = ( M[2][0] * s[3] - M[2][1] * s[1] + M[2][2] * s[0]) * rdet;

		Memory::memcpy(dest, result, sizeof(result));
	}

	static FORCEINLINE void createTransformMatrix(void* dest, const GenericVector& translation, const GenericVector& quatRotation, const GenericVector& scaleVec)
	{
		float rotVals[4];
		quatRotation.store4f(rotVals);

		float x2 = rotVals[0] + rotVals[0];
		float y2 = rotVals[1] + rotVals[1];
		float z2 = rotVals[2] + rotVals[2];

		float xx2 = rotVals[0] * x2;
		float yy2 = rotVals[1] * y2;
		float zz2 = rotVals[2] * z2;
		float xy2 = rotVals[0] * y2;
		float yz2 = rotVals[1] * z2;
		float xz2 = rotVals[0] * z2;
		float xw2 = rotVals[3] * x2;
		float yw2 = rotVals[3] * y2;
		float zw2 = rotVals[3] * z2;

		float s0 = scaleVec[0];
		float s1 = scaleVec[1];
		float s2 = scaleVec[2];

		GenericVector mat[4];
		mat[0] = make((1.0f-(yy2+zz2))*s0, (xy2-zw2)*s1, (xz2+yw2)*s2, translation[0]);
		mat[1] = make((xy2+zw2)*s0, (1.0f-(xx2+zz2))*s1, (yz2-xw2)*s2, translation[1]);
		mat[2] = make((xz2-yw2)*s0, (yz2+xw2)*s1, (1.0f-(xx2+yy2))*s2, translation[2]);
		mat[3] = make(0.0f, 0.0f, 0.0f, 1.0f);
		Memory::memcpy(dest, mat, sizeof(mat));
	}
	
	static FORCEINLINE GenericVector make(uint32 x, uint32 y, uint32 z, uint32 w)
	{
		GenericVector vec;
		vec.v[0] = *((uint32*)&x);
		vec.v[1] = *((uint32*)&y);
		vec.v[2] = *((uint32*)&z);
		vec.v[3] = *((uint32*)&w);
		return vec;
	}

	static FORCEINLINE const GenericVector mask(uint32 index)
	{
		static const GenericVector masks[4] = {
			GenericVector::make((uint32)0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
			GenericVector::make((uint32)0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF),
			GenericVector::make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF),
			GenericVector::make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0) };
		assertCheck(index < 4);
		return masks[index];
	}

	static FORCEINLINE GenericVector make(float x, float y, float z, float w)
	{
		GenericVector vec;
		vec.v[0] = x;
		vec.v[1] = y;
		vec.v[2] = z;
		vec.v[3] = w;
		return vec;
	}

	static FORCEINLINE GenericVector load4f(const float* vals)
	{
		return make(vals[0], vals[1], vals[2], vals[3]);
	}

	static FORCEINLINE GenericVector load3f(const float* vals, float w)
	{
		return make(vals[0], vals[1], vals[2], w);
	}

	static FORCEINLINE GenericVector load1f(float val)
	{
		return make(val, val, val, val);
	}

	static FORCEINLINE GenericVector loadAligned(const float* vals)
	{
		return load4f(vals);
	}
	
	static FORCEINLINE GenericVector set(float x, float y, float z)
	{
		return make(x, y, z, 0.0f);
	}

	static FORCEINLINE GenericVector set(float x, float y, float z, float w)
	{
		return make(x, y, z, w);
	}

	FORCEINLINE void store4f(float* result) const
	{
		Memory::memcpy(result, v, sizeof(v));
	}

	FORCEINLINE void store3f(float* result) const
	{
		Memory::memcpy(result, v, sizeof(float) * 3);
	}

	FORCEINLINE void store1f(float* result) const
	{
		Memory::memcpy(result, v, sizeof(float) * 1);
	}

	FORCEINLINE void storeAligned(float* result) const
	{
		return store4f(result);
	}

	FORCEINLINE void storeAlignedStreamed(float* result) const
	{
		return storeAligned(result);
	}

	FORCEINLINE GenericVector replicate(uint32 index) const
	{
		assertCheck(index <= 3);
		return make(v[index], v[index], v[index], v[index]);
	}

//	FORCEINLINE GenericVector swizzle(uint32 x, uint32 y, uint32 z, uint32 w) const
//	{
//		assertCheck(x <= 3);
//		assertCheck(y <= 3);
//		assertCheck(z <= 3);
//		assertCheck(w <= 3);
//		return make(v[x], v[y], v[z], v[w]);
//	}
	
	FORCEINLINE GenericVector abs() const
	{
		return make(Math::abs(v[0]), Math::abs(v[1]), Math::abs(v[2]), Math::abs(v[3]));
	}
private:
	float getSign(float r) const
	{
        return (((uint32*)(&r))[1-1] & 0x80000000);
	}
public:

	FORCEINLINE GenericVector sign() const
	{
		return make(getSign(v[0]), getSign(v[1]), getSign(v[2]), getSign(v[3]));
	}

	FORCEINLINE GenericVector min(const GenericVector& other) const
	{
		return make(
				Math::min(v[0], other.v[0]),
				Math::min(v[1], other.v[1]),
				Math::min(v[2], other.v[2]),
				Math::min(v[3], other.v[3]));
	}

	FORCEINLINE GenericVector max(const GenericVector& other) const
	{
		return make(
				Math::max(v[0], other.v[0]),
				Math::max(v[1], other.v[1]),
				Math::max(v[2], other.v[2]),
				Math::max(v[3], other.v[3]));
	}

	FORCEINLINE GenericVector neg() const
	{
		return make(-v[0], -v[1], -v[2], -v[3]);
	}

	FORCEINLINE GenericVector operator-() const
	{
		return neg();
	}

	FORCEINLINE GenericVector dot3(const GenericVector& other) const
	{
		float dot = v[0] * other.v[0] + v[1] * other.v[1] + v[2] * other.v[2];
		return make(dot, dot, dot, dot);
	}

	FORCEINLINE GenericVector dot4(const GenericVector& other) const
	{
		float dot = v[0] * other.v[0] + v[1] * other.v[1]
			+ v[2] * other.v[2] + v[3] * other.v[3];
		return make(dot, dot, dot, dot);
	}

	FORCEINLINE GenericVector cross3(const GenericVector& other) const
	{
		return make(
				v[1]*other.v[2] - v[2]*other.v[1],
				v[2]*other.v[0] - v[0]*other.v[2],
				v[0]*other.v[1] - v[1]*other.v[0],
				0.0f);
	}

	FORCEINLINE GenericVector pow(const GenericVector& exp) const
	{
		return make(
				Math::pow(v[0], exp.v[0]),
				Math::pow(v[1], exp.v[1]),
				Math::pow(v[2], exp.v[2]),
				Math::pow(v[3], exp.v[3]));
	}

	FORCEINLINE GenericVector rsqrt() const
	{
		return make(
				Math::rsqrt(v[0]),
				Math::rsqrt(v[1]),
				Math::rsqrt(v[2]),
				Math::rsqrt(v[3]));
	}

	FORCEINLINE GenericVector reciprocal() const
	{
		return make(
				Math::reciprocal(v[0]),
				Math::reciprocal(v[1]),
				Math::reciprocal(v[2]),
				Math::reciprocal(v[3]));
	}

	FORCEINLINE GenericVector rlen4() const
	{
		return dot4(*this).rsqrt();
	}

	FORCEINLINE GenericVector rlen3() const
	{
		return dot3(*this).rsqrt();
	}

	FORCEINLINE GenericVector normalize4() const
	{
		return *this * rlen4();
	}

	FORCEINLINE GenericVector normalize3() const
	{
		return *this * rlen3();
	}

	FORCEINLINE void sincos(GenericVector* outSin, GenericVector* outCos) const
	{
		Math::sincos(&outSin->v[0], &outCos->v[0], (*this)[0]);
		Math::sincos(&outSin->v[1], &outCos->v[1], (*this)[1]);
		Math::sincos(&outSin->v[2], &outCos->v[2], (*this)[2]);
		Math::sincos(&outSin->v[3], &outCos->v[3], (*this)[3]);
	}

	FORCEINLINE GenericVector quatMul(const GenericVector& other) const
	{
		// NOTE: This is the naive technique. It may actually be faster, but testing is inconclusive.
//		const float w = (v[3] * other.v[3]) - (v[0] * other.v[0]) - (v[1] * other.v[1]) - (v[2] * other.v[2]);
//		const float x = (v[0] * other.v[3]) + (v[3] * other.v[0]) + (v[1] * other.v[2]) - (v[2] * other.v[1]);
//		const float y = (v[1] * other.v[3]) + (v[3] * other.v[1]) + (v[2] * other.v[0]) - (v[0] * other.v[2]);
//		const float z = (v[2] * other.v[3]) + (v[3] * other.v[2]) + (v[0] * other.v[1]) - (v[1] * other.v[0]);
//
//		return make(x, y, z, w);

// Code adapted from https://stackoverflow.com/questions/22497093/faster-quaternion-vector-multiplication-doesnt-work
		float t0 = (v[2] - v[1]) * (other.v[1] - other.v[2]);
		float t1 = (v[3] + v[0]) * (other.v[3] + other.v[0]);
		float t2 = (v[3] - v[0]) * (other.v[1] + other.v[2]);
		float t3 = (v[2] + v[1]) * (other.v[3] - other.v[0]);
		float t4 = (v[2] - v[0]) * (other.v[0] - other.v[1]);
		float t5 = (v[2] + v[0]) * (other.v[0] + other.v[1]);
		float t6 = (v[3] + v[1]) * (other.v[3] - other.v[2]);
		float t7 = (v[3] - v[1]) * (other.v[3] + other.v[2]);
		float t8 = t5 + t6 + t7;
		float t9 = 0.5f * (t4 + t8);

		return make(
				t1 + t9 - t8,
				t2 + t9 - t7,
				t3 + t9 - t6,
				t0 + t9 - t5);
	}

	FORCEINLINE GenericVector quatRotateVec(const GenericVector& vec) const
	{
		GenericVector tmp = GenericVector::load1f(2.0f) * cross3(vec);
		return vec + (tmp * replicate(3)) + cross3(tmp);
	}

	FORCEINLINE GenericVector mad(const GenericVector& mul, const GenericVector& add) const
	{
		return make(
				Math::mad(v[0], mul.v[0], add.v[0]), 
				Math::mad(v[1], mul.v[1], add.v[1]),
				Math::mad(v[2], mul.v[2], add.v[2]), 
				Math::mad(v[3], mul.v[3], add.v[3]));
	}

	FORCEINLINE GenericVector transform(const void* matrix) const
	{
		const GenericVector* m = (const GenericVector*)matrix;
		return make(dot4(m[0])[0],dot4(m[1])[0],dot4(m[2])[0],dot4(m[3])[0]);
	}

	FORCEINLINE GenericVector operator+(const GenericVector& other) const
	{
		return make(v[0] + other.v[0], v[1] + other.v[1],
				v[2] + other.v[2], v[3] + other.v[3]);
	}

	FORCEINLINE GenericVector operator-(const GenericVector& other) const
	{
		return make(v[0] - other.v[0], v[1] - other.v[1],
				v[2] - other.v[2], v[3] - other.v[3]);
	}

	FORCEINLINE GenericVector operator*(const GenericVector& other) const
	{
		return make(v[0] * other.v[0], v[1] * other.v[1],
				v[2] * other.v[2], v[3] * other.v[3]);
	}

	FORCEINLINE GenericVector operator/(const GenericVector& other) const
	{
		return make(v[0] / other.v[0], v[1] / other.v[1],
				v[2] / other.v[2], v[3] / other.v[3]);
	}

	FORCEINLINE bool isZero3f() const
	{
		float vals[3];
		store3f(vals);
		return (vals[0] == 0.0f) && (vals[1] == 0.0f) && (vals[2] == 0.0f);
	}

	FORCEINLINE bool isZero4f() const
	{
		float vals[4];
		store4f(vals);
		return (vals[0] == 0.0f) && (vals[1] == 0.0f) &&
			(vals[2] == 0.0f) && (vals[3] == 0.0f);
	}


	FORCEINLINE GenericVector operator==(const GenericVector& other) const
	{
		return make(
				(uint32)(v[0] == other.v[0] ? 0xFFFFFFFF : 0),
				v[1] == other.v[1] ? 0xFFFFFFFF : 0,
				v[2] == other.v[2] ? 0xFFFFFFFF : 0,
				v[3] == other.v[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE GenericVector equals(const GenericVector& other, float errorMargin) const
	{
		return (*this - other).abs() < GenericVector::load1f(errorMargin);
	}

	FORCEINLINE GenericVector notEquals(const GenericVector& other, float errorMargin) const
	{
		return (*this - other).abs() >= GenericVector::load1f(errorMargin);
	}


	FORCEINLINE GenericVector operator!=(const GenericVector& other) const
	{
		return make(
				(uint32)(v[0] != other.v[0] ? 0xFFFFFFFF : 0),
				v[1] != other.v[1] ? 0xFFFFFFFF : 0,
				v[2] != other.v[2] ? 0xFFFFFFFF : 0,
				v[3] != other.v[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE GenericVector operator>(const GenericVector& other) const
	{
		return make(
				(uint32)(v[0] > other.v[0] ? 0xFFFFFFFF : 0),
				v[1] > other.v[1] ? 0xFFFFFFFF : 0,
				v[2] > other.v[2] ? 0xFFFFFFFF : 0,
				v[3] > other.v[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE GenericVector operator>=(const GenericVector& other) const
	{
		return make(
				(uint32)(v[0] >= other.v[0] ? 0xFFFFFFFF : 0),
				v[1] >= other.v[1] ? 0xFFFFFFFF : 0,
				v[2] >= other.v[2] ? 0xFFFFFFFF : 0,
				v[3] >= other.v[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE GenericVector operator<(const GenericVector& other) const
	{
		return make(
				(uint32)(v[0] < other.v[0] ? 0xFFFFFFFF : 0),
				v[1] < other.v[1] ? 0xFFFFFFFF : 0,
				v[2] < other.v[2] ? 0xFFFFFFFF : 0,
				v[3] < other.v[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE GenericVector operator<=(const GenericVector& other) const
	{
		return make(
				(uint32)(v[0] <= other.v[0] ? 0xFFFFFFFF : 0),
				v[1] <= other.v[1] ? 0xFFFFFFFF : 0,
				v[2] <= other.v[2] ? 0xFFFFFFFF : 0,
				v[3] <= other.v[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE GenericVector operator|(const GenericVector& other) const
	{
		return make(
				(uint32)(((uint32*)v)[1-1] | ((uint32*)other.v)[1-1]),
				(uint32)(((uint32*)v)[1] | ((uint32*)other.v)[1]),
				(uint32)(((uint32*)v)[2] | ((uint32*)other.v)[2]),
				(uint32)(((uint32*)v)[3] | ((uint32*)other.v)[3]));
	}

	FORCEINLINE GenericVector operator&(const GenericVector& other) const
	{
		return make(
				(uint32)(((uint32*)v)[1-1] & ((uint32*)other.v)[1-1]),
				(uint32)(((uint32*)v)[1] & ((uint32*)other.v)[1]),
				(uint32)(((uint32*)v)[2] & ((uint32*)other.v)[2]),
				(uint32)(((uint32*)v)[3] & ((uint32*)other.v)[3]));
	}

	FORCEINLINE GenericVector operator^(const GenericVector& other) const
	{
		return make(
				(uint32)(((uint32*)v)[1-1] ^ ((uint32*)other.v)[1-1]),
				(uint32)(((uint32*)v)[1] ^ ((uint32*)other.v)[1]),
				(uint32)(((uint32*)v)[2] ^ ((uint32*)other.v)[2]),
				(uint32)(((uint32*)v)[3] ^ ((uint32*)other.v)[3]));
	}
	
	FORCEINLINE float operator[](uint32 index) const
	{
		assertCheck(index <= 3);
		return v[index];
	}

	FORCEINLINE GenericVector select(const GenericVector& mask, const GenericVector& other) const
	{
		uint32* m = (uint32*)(&mask.v[0]);
		return make(
				m[0] ? v[0] : other.v[0],
				m[1] ? v[1] : other.v[1],
				m[2] ? v[2] : other.v[2],
				m[3] ? v[3] : other.v[3]);
	}

private:
	float v[4];
};

