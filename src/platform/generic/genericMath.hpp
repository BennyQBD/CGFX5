#pragma once

#include "../../core/common.hpp"
#include "cmwc4096.h"
#include <cmath>
#include <cstdlib>

/**
 * Various math functions. 
 *
 * Rather than use C's math functions directly, they are encapsulated in a
 * this struct so they can potentially be optimized for various platforms.
 *
 * Additionally, this struct contains a few math functions that are not part
 * of C's math library.
 */
struct GenericMath
{
	static CONSTEXPR const float PI = 3.1415926535897932f;
	static CONSTEXPR const float TWO_PI = 6.28318530717959f;
	static CONSTEXPR const float HALF_PI = 1.57079632679f;
	static CONSTEXPR const float R_PI = 0.31830988618f;
	static CONSTEXPR const float R_TWO_PI = 0.159154943091895f;
	static CONSTEXPR const float R_HALF_PI = 0.636619772367581f;

	static CONSTEXPR const float E = 2.71828182845904523536f;
	static CONSTEXPR const float R_LN_2 = 1.44269504088896f;
	static CONSTEXPR const float RAD_TO_DEG_CONV = 57.2957795130823f; // 180.0f/PI;
	static CONSTEXPR const float DEG_TO_RAD_CONV = 0.0174532925199433f; // PI/180.0f;

	static CONSTEXPR inline int32 truncToInt(float val)
	{
		return (int32)val;
	}

	static CONSTEXPR inline float truncToFloat(float val)
	{
		return (float)truncToInt(val);
	}

	static inline int32 floorToInt(float val)
	{
		return truncToInt(floorToFloat(val));
	}

	static inline float floorToFloat(float val)
	{
		return floorf(val);
	}

	static inline double floorToDouble(double val)
	{
		return floor(val);
	}

	static inline int32 roundToInt(float val)
	{
		return ceilToInt(val - 0.5f);
	}

	static inline float roundToFloat(float val)
	{
		return ceilToFloat(val - 0.5f);
	}

	static inline int32 ceilToInt(float val)
	{
		return truncToInt(ceilf(val));
	}

	static inline float ceilToFloat(float val)
	{
		return ceilf(val);
	}

	static inline double ceilToDouble(double val)
	{
		return ceil(val);
	}

	static inline float fractional(float val)
	{
		return val - truncToFloat(val);
	}

	static inline float exp(float val) { return expf(val); }
	static inline float ln(float val) { return logf(val); }
	static inline float logx(float base, float val) { return ln(val)/ln(base); }
	static inline float log2(float val) { return ln(val) * R_LN_2; }

	static inline float fmod(float num, float den) { return fmodf(num, den); }
	static inline float sin(float val) { return sinf(val); }
	static inline float asin(float val) { return asinf(val); }
	static inline float cos(float val) { return cosf(val); }
	static inline float acos(float val) { return acosf(val); }
	static inline float tan(float val) { return tanf(val); }
	static inline float atan(float val) { return atanf(val); }
	static inline float atan(float val, float val2) { return atan2f(val, val2); }
	static inline float pow(float val, float power) { return powf(val, power); }
	static inline float sqrt(float val) { return sqrtf(val); }

	static inline void sincos(float* outSin, float* outCos, float angle)
	{
		// If angle is outside range of [0, 2*pi], adjust it so it is.
		// Using fmod gives worse precision than the following code
		if((angle < 0.0f) || (angle >= TWO_PI)) {
			angle -= floorToFloat(angle * R_TWO_PI) * TWO_PI;
		}
//		angle = Math::fmod(angle, TWO_PI);

		// This approximation is only accurate with angles in the range
		// [-pi/2, pi/2]. If angle is outside that range, convert it to that
		// range and find the appropriate sign to adjust the result.
		angle = PI - angle;
		float sign = -1.0f;
		if (fabsf(angle) >= HALF_PI ) {
			angle = select(angle, PI, -PI) - angle;
			sign = 1.0f;
		}

		// Sine and cosine are calculated with their respective taylor series
		// (technically Maclaurin series, since no offset is used). The "typical"
		// series is as follows:
		//
		// sin(a) = a/(1!) - a^3/(3!) + a^5/(5!) - a^7/(7!) + ...
		// cos(a) = 1 - a^2/(2!) + a^4/(4!) - a^6/(6!) + a^8/(8!) - ...
		//
		// However, this form is inefficient for computation. Some algebra has 
		// been performed to reduce computational cost.
		//
		// For sine:
		// - The a was factored out
		// sin(a) = a * (1/(1!) - a^2/(3!) + a^4/(5!) - a^6/(7!))
		// - a^2 was factored out
		// sin(a) = a * (1/(1!) + a^2*(-1/(3!) + a^2*(1/(5!) + a^2*(...))))
		// - The factorial constants were precalculated and hardcoded, leaving
		// the final equation used. The equation terminates at the a^9/(9!) term
		// of the original taylor series.
		//
		// For cosine:
		// - a^2 was factored out
		// cos(a) = 1 + a^2*(-1/(2!) + a^2*(1/(4!) + a^2*(-1/(6!) + a^2*(...))))
		// - The factorial constants were precalculated and hardcoded, leaving
		// the final equation used. The equation terminates at the a^10/(10!)
		// term of the original taylor series.
		//
		// Note that in both equations, some of the factorial constants have
		// been modified to generate empirically more accurate results.

		float a2 = angle * angle;
		*outSin = angle * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) *
						a2 + 8.3333315e-03f) * a2 - 1.666666664e-01f) * a2 + 1.0f);
		*outCos = sign * (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 1.3888397e-03f) *
						a2 + 4.16666418e-02f) * a2 - 4.999999963e-01f) * a2 + 1.0f);
	}

	static inline float reciprocal(float val)
	{
		return 1.0f/val;
	}

	static inline float rsqrt(float val)
	{
		return reciprocal(sqrtf(val));
	}

	static inline bool isNaN(float val)
	{
		union {
			float f;
			uint32 i;
		} f;
		f.f = val;
		return (f.i & 0x7FFFFFFF) > 0x7F800000;
	}

	static inline bool isFinite(float val)
	{
		union {
			float f;
			uint32 i;
		} f;
		f.f = val;
		return (f.i & 0x7F800000) != 0x7F800000;
	}

	static inline int32 rand() { return ::rand(); }
	static inline void seedRand(int32 seed) { srand((uint32)seed); }
	static inline float randf() { return ::rand()/(float)RAND_MAX; }
	
	static inline uint32 floorLog2(uint32 val)
	{
		uint32 pos = 0;
		if (val >= 1<<16) { val >>= 16; pos += 16; }
		if (val >= 1<< 8) { val >>=  8; pos +=  8; }
		if (val >= 1<< 4) { val >>=  4; pos +=  4; }
		if (val >= 1<< 2) { val >>=  2; pos +=  2; }
		if (val >= 1<< 1) {				pos +=  1; }
		return (val == 0) ? 0 : pos;
	}

	static inline uint32 getNumLeadingZeroes(uint32 val)
	{
		if(val == 0) {
			return 32;
		}
		return 31 - floorLog2(val);
	}

	static inline uint32 ceilLog2(uint32 val)
	{
		if(val <= 1) {
			return 0;
		}
		return floorLog2(val-1)+1;
	}

	static inline uint32 roundUpToNextPowerOf2(uint32 val)
	{
		return 1 << ceilLog2(val);
	}

	template<typename T>
	static CONSTEXPR inline T select(const T& cmp,
			const T& valIfGreaterOrEqualToZero,
			const T& valIfLessZero)
	{
		return cmp >= (T)(0) ? valIfGreaterOrEqualToZero : valIfLessZero;
	}

	template<typename T>
	static CONSTEXPR inline T abs(const T& val)
	{
		return val >= (T)(0) ? val : -val;
	}

// WARNING: This behavior is not consistent with Vector sign function!
//	template<typename T>
//	static CONSTEXPR inline T sign(const T& val)
//	{
//		return (T(0) < val) - (val < T(0));
//	}

	template<typename T>
	static CONSTEXPR inline T min(const T& val1, const T& val2)
	{
		return val1 <= val2 ? val1 : val2;
	}

	template<typename T>
	static CONSTEXPR inline T min3(const T& val1, const T& val2, const T& val3)
	{
		return min(min(val1, val2), val3);
	}

	template<typename T>
	static CONSTEXPR inline T max(const T& val1, const T& val2)
	{
		return val1 >= val2 ? val1 : val2;
	}

	template<typename T>
	static CONSTEXPR inline T max3(const T& val1, const T& val2, const T& val3)
	{
		return max(max(val1, val2), val3);
	}

	template<typename T>
	static inline T mad(const T& val1, const T& val2, const T& val3)
	{
		return val1 * val2 + val3;
	}

	template<typename T>
	static inline T toDegrees(const T& val)
	{
		return val * RAD_TO_DEG_CONV;
	}

	template<typename T>
	static inline T toRadians(const T& val)
	{
		return val * DEG_TO_RAD_CONV;
	}

	template<typename T, typename U>
	static inline T lerp(const T& val1, const T& val2, const U& amt)
	{
		return (T)(val1 * ((U)(1) - amt) + val2 * amt);
	}

	template<typename T, typename U>
	static inline T cubicLerp(const T& val1, const T& val2, const U& amt)
	{
		// Cubic interp, assuming tangents are 0 at all points.
		return lerp(val1, val2, 3*amt*amt - 2*amt*amt*amt);
	}

	template<typename T, typename U>
	static inline T cubicInterp(const T& val0, const T& val1, const T& val2, const T& val3, const U& amt)
	{
		// Using Catmull-Rom tangents
		U amt2 = amt*amt;
		return (
				 (val3*(U(1)/U(2)) - val2*(U(3)/U(2)) - val0*(U(1)/U(2)) + val1*(U(3)/U(2)))*amt*amt2
				+(val0 - val1*(U(5)/U(2)) + val2*U(2) - val3*(U(1)/U(2)))*amt2
				+(val2*(U(1)/U(2)) - val0*(U(1)/U(2)))*amt
				+val1);
	}

	template<typename T, typename U>
	static inline T cubicInterpSpecifyTangents(const T& val1, const T& tan1, const T& val2, const T& tan2, const U& amt)
	{
		// Using custom tangents
		U amt2 = amt*amt;
		return (
				 (tan2 - val2*U(2) + tan1 + val1*(U(2)))*amt*amt2
				+(tan1*U(2) - val1*U(3) + val2*U(3) - tan2*U(2))*amt2
				+tan1*amt
				+val1);
	}

	template<typename T, typename U>
	static inline T bilerp(const T& val00, const T& val10,
			const T& val01, const T& val11,
			const U& amtX, const U& amtY)
	{
		return lerp(
				lerp(val00, val10, amtX),
				lerp(val01, val11, amtX),
				amtY);
	}

	template<typename T>
	static inline T clamp(const T& val, const T& min, const T& max)
	{
		if(val > max) {
			return max;
		} else if(val > min) {
			return val;
		} else {
			return min;
		}
	}

	template<typename T>
	static inline T saturate(const T& val)
	{
		return clamp(val, (T)(0), (T)(1));
	}

	template<typename T>
	static inline T square(const T& val)
	{
		return val*val;
	}

	template<typename T>
	static inline T cube(const T& val)
	{
		return val*val*val;
	}

	template<typename T>
	static inline bool equals(const T& val1, const T& val2, const T& errorMargin)
	{
		return abs(val1 - val2) < errorMargin;
	}

	// TODO: Min and max of array
};

template<>
inline float GenericMath::abs(const float& val)
{
	return fabsf(val);
}


