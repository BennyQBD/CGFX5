#pragma once

#include "platform.hpp"

//Include appropriate header files for SIMD features and CPU architecture
#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
	#if SIMD_SUPPORTED_LEVEL >= SIMD_LEVEL_x86_AVX2
		#ifdef __GNUC__
			#include <x86intrin.h>
		#else
			#include <immintrin.h>
		#endif
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_AVX
		#include <immintrin.h>
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE4_2
		#include <nmmintrin.h>
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE4_1
		#include <smmintrin.h>
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSSE3
		#include <tmmintrin.h>
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE3
		#include <pmmintrin.h>
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE2
		#include <emmintrin.h>
	#elif SIMD_SUPPORTED_LEVEL == SIMD_LEVEL_x86_SSE
		#include <xmmintrin.h>
	#endif
#endif

