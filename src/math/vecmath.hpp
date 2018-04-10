#pragma once

#include "core/memory.hpp"
#include "platform/platformVecmath.hpp"
#include "math.hpp"

typedef PlatformVector Vector;

struct VectorConstants
{
public:
	static const Vector ZERO;
	static const Vector ONE;
	static const Vector TWO;
	static const Vector HALF;
	static const Vector INF;
	static const Vector MASK_X;
	static const Vector MASK_Y;
	static const Vector MASK_Z;
	static const Vector MASK_W;
	static const Vector SIGN_MASK;
};

template<>
inline Vector Math::lerp(const Vector& val1, const Vector& val2,
		const float& amt)
{
	return (val2 - val1) * Vector::load1f(amt) + val1;
}

