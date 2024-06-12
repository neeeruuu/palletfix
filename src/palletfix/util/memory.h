#pragma once

#include <vcruntime.h>

namespace mem {
#ifdef _AMD64_
	/*
		get pointer to RIP relative address
	*/
	__forceinline unsigned __int64 getPtr(unsigned __int64 dwAddress, unsigned __int32 iOffset = 0) {
		return dwAddress + *(__int32*)(dwAddress + iOffset) + iOffset + sizeof(__int32);
	}
	/*
		read RIP relative pointer
	*/
	__forceinline unsigned __int64 readPtr(unsigned __int64 dwAddress, unsigned __int32 iOffset = 0) {
		return *reinterpret_cast<unsigned __int64*>(getPtr(dwAddress, iOffset));
	}

#endif

	/*
		Scans for a pattern on the specified module handle
	*/
	intptr_t findPattern(void* modHandle, const char* pattern, const char* mask, bool isDirectRef = false);

	/*
		converts data to little endian
	*/
	template <typename T>
	__forceinline void toLittleEndian(unsigned char* dest, T value) {
		for (size_t i = 0; i < sizeof(value); ++i)
			dest[i] = (value >> (8 * i)) & 0xFF;
	}
}