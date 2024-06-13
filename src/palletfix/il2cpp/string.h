#pragma once

#include <vcruntime.h>
#include <wchar.h>

#include "base.h"

/*
	functions obtained on offsets.h
*/
class System_String;
namespace _internal {
	inline System_String* (*String__Concat)(System_String*, System_String*, void*);
	inline System_String* (*String__Copy)(System_String*, void*);
	inline System_String* (*String__New)(const char* str);
	inline System_String* (*String__New_UTF16)(const wchar_t* str, int len);
}

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

	void write(const wchar_t* buff);

	System_String* concat(System_String* str) {
		return _internal::String__Concat(this, str, 0);
	}

	System_String* copy() {
		return _internal::String__Copy(this, 0);
	}

	static System_String* create(const char* buff) {
		return _internal::String__New(buff);
	}

	static System_String* create(const wchar_t* buff, int len) {
		return _internal::String__New_UTF16(buff, len);
	}
};