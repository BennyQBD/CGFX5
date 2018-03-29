#pragma once

#include "common.hpp"
#include "../platform/platformMemory.hpp"
#include <cstring>

/**
 * Various memory functions. 
 *
 * Rather than use C's memory functions directly, they are encapsulated in a
 * this struct so they can potentially be optimized for various platforms.
 *
 * Additionally, this struct contains a few memory functions that are not part
 * of C's memory library.
 */
struct Memory
{
	static FORCEINLINE void* memmove(void* dest, const void* src, uintptr amt)
	{
		return PlatformMemory::memmove(dest, src, amt);
	}

	static FORCEINLINE int32 memcmp(const void* dest, const void* src, uintptr amt)
	{
		return PlatformMemory::memcmp(dest, src, amt);
	}

	template<typename T>
	static FORCEINLINE void* memset(void* dest, T val, uintptr amt)
	{
		return PlatformMemory::memset(dest, val, amt);
	}

	static FORCEINLINE void* memzero(void* dest, uintptr amt)
	{
		return PlatformMemory::memset(dest, 0, amt);
	}

	static FORCEINLINE void* memcpy(void* dest, const void* src, uintptr amt)
	{
		return PlatformMemory::memcpy(dest, src, amt);
	}

	static FORCEINLINE void memswap(void* a, void* b, uintptr size)
	{
		return PlatformMemory::memswap(a, b, size);
	}

	enum 
	{
		DEFAULT_ALIGNMENT = 0,
		MIN_ALIGNMENT = 8
	};

	template<typename T>
	static inline CONSTEXPR T align(const T ptr, uintptr alignment)
	{
		return PlatformMemory::align(ptr, alignment);
	}

	static FORCEINLINE void* malloc(uintptr amt, uint32 alignment=DEFAULT_ALIGNMENT)
	{
		return PlatformMemory::malloc(amt, alignment);
	}

	static FORCEINLINE void* realloc(void* ptr, uintptr amt, uint32 alignment=DEFAULT_ALIGNMENT)
	{
		return PlatformMemory::realloc(ptr, amt, alignment);
	}

	static FORCEINLINE void* free(void* ptr)
	{
		return PlatformMemory::free(ptr);
	}

	static FORCEINLINE uintptr getAllocSize(void* ptr)
	{
		return PlatformMemory::getAllocSize(ptr);
	}
};
