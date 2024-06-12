#pragma once

#include <vcruntime.h>
#include <combaseapi.h>

#include "interop.h"

struct System_String_fields {
	__int32 _strlen;
	wchar_t _chars[];
};

class IL2CPPString {
private:
	const intptr_t _klass;
	const intptr_t _monitor;
	System_String_fields _fields;

	IL2CPPString();
public:
	__forceinline int length() const { return _fields._strlen; }
	__forceinline int size() const { return (_fields._strlen + 1) * sizeof(wchar_t); }
	__forceinline const wchar_t* c_str() const { return _fields._chars; }

	bool operator==(const IL2CPPString& str) const {
		if (str._fields._strlen != _fields._strlen)
			return false;
		if (lstrcmpW(str._fields._chars, _fields._chars) == 0)
			return true;
		return false;
	}

	IL2CPPString* concat(IL2CPPString* str) {
		static auto fn = reinterpret_cast<IL2CPPString * (*)(IL2CPPString*, IL2CPPString*, void*)>(interop::_addresses::String_Concat);
		return fn(this, str, 0);
	}

	IL2CPPString* copy() {
		static auto fn = reinterpret_cast<IL2CPPString * (*)(IL2CPPString*, void*)>(interop::_addresses::String_Copy);
		return fn(this, 0);
	}

	static IL2CPPString* create(int len) {
		static auto fn = reinterpret_cast<IL2CPPString * (*)(IL2CPPString*, void*, int, int, void*, void*)>(interop::_addresses::String_Create);
		return fn(0, 0, 0, len, 0, 0);
	}
};