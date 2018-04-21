#pragma once

#include "core/common.hpp"
#include <cstring>

struct GenericMemory
{
	static FORCEINLINE void* memmove(void* dest, const void* src, uintptr amt)
	{
		return ::memmove(dest, src, amt);
	}

	static FORCEINLINE int32 memcmp(const void* dest, const void* src, uintptr amt)
	{
		return ::memcmp(dest, src, amt);
	}

	template<typename T>
	static FORCEINLINE void* memset(void* destIn, T val, uintptr amt)
	{
		T* dest = (T*)destIn;
		uintptr amtT = amt/sizeof(T);
		uintptr remainder = amt % sizeof(T);
		for(uintptr i = 0; i < amtT; ++i, ++dest) {
			memcpy(dest, &val, sizeof(T));
		}
		memcpy(dest, &val, remainder);
		return destIn;
	}

	static FORCEINLINE void* memzero(void* dest, uintptr amt)
	{
		return ::memset(dest, 0, amt);
	}

	static FORCEINLINE void* memcpy(void* dest, const void* src, uintptr amt)
	{
		return ::memcpy(dest, src, amt);
	}

	static void memswap(void* a, void* b, uintptr size)
	{
		char* temp_data = new char[size];
		void* temp = (void*)&temp_data;
		GenericMemory::memcpy(temp, a, size);
		GenericMemory::memcpy(a, b, size);
		GenericMemory::memcpy(b, temp, size);

		delete[] temp_data;
	}

	template<typename T>
	static FORCEINLINE CONSTEXPR T align(const T ptr, uintptr alignment)
	{
		return (T)(((intptr)ptr + alignment - 1) & ~(alignment-1));
	}

	static void* malloc(uintptr amt, uint32 alignment);
	static void* realloc(void* ptr, uintptr amt, uint32 alignment);
	static void* free(void* ptr);
	static uintptr getAllocSize(void* ptr);
};

template<>
FORCEINLINE void* GenericMemory::memset(void* dest, uint8 val, uintptr amt)
{
	return ::memset(dest, val, amt);
}

