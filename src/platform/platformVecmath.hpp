#pragma once

#include "platform.hpp"

#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
#include "sse/sseVecmath.hpp"
	typedef SSEVector PlatformVector;
#else
#include "generic/genericVecmath.hpp"
	typedef GenericVector PlatformVector;
#endif
