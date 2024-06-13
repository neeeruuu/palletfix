#pragma once

#include <vcruntime.h>
#include <wchar.h>

#include "interop.h"
#include "base.h"

struct System_String_fields {
	__int32 _strlen;
	wchar_t _chars[1];
};

class System_String : Il2CppObject<System_String_fields> {
private:
	System_String();
public:
	__forceinline int length() const { return _fields._strlen; }
	__forceinline int size() const { return (_fields._strlen + 1) * sizeof(wchar_t); }
	__forceinline const wchar_t* c_str() const { return _fields._chars; }

	bool operator==(const System_String& str) const {
		if (str._fields._strlen != _fields._strlen)
			return false;
		if (wcscmp(str._fields._chars, _fields._chars) == 0)
			return true;
		return false;
	}

	System_String* concat(System_String* str) {
		static auto fn = reinterpret_cast<System_String * (*)(System_String*, System_String*, void*)>(interop::_addresses::String_Concat);
		return fn(this, str, 0);
	}

	System_String* copy() {
		static auto fn = reinterpret_cast<System_String * (*)(System_String*, void*)>(interop::_addresses::String_Copy);
		return fn(this, 0);
	}

	static System_String* create(int len) {
		static auto fn = reinterpret_cast<System_String * (*)(System_String*, void*, int, int, void*, void*)>(interop::_addresses::String_Create);
		return fn(0, 0, 0, len, 0, 0);
	}
};