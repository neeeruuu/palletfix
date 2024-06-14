#pragma once

#include <vcruntime.h>
#include <wchar.h>

#include "base.h"

struct System_String_fields {
	__int32 _strlen;
	wchar_t _chars[1];
};

class System_String : protected Il2CppObject<System_String_fields> {
private:
	System_String();
public:
	__forceinline int length() const { return _fields._strlen; }
	__forceinline int size() const { return (_fields._strlen + 1) * sizeof(wchar_t); }
	__forceinline const wchar_t* c_str() const { return _fields._chars; }

	static System_String* create();
	static System_String* create(const char* buff);
	static System_String* create(const wchar_t* buff);

	System_String* concat(System_String* str);
	System_String* copy();

	bool operator==(const System_String& str) const;
};