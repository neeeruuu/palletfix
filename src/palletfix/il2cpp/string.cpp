#include "string.h"
#include "globals.h"
#include "il2cpp.h"

#include <libloaderapi.h>

System_String* System_String::create()
{
	static Il2CppClass* stringClass = il2cpp::classFromName("mscorlib", "System", "String");
	return reinterpret_cast<System_String*>(il2cpp::create(stringClass));
}

System_String* System_String::create(const char* buff)
{
	static System_String* (*_new)(const char* str);
	if (!_new)
		_new = reinterpret_cast<decltype(_new)>(GetProcAddress(reinterpret_cast<HMODULE>(g_GameAssembly), "il2cpp_string_new"));
	return _new(buff);
}

System_String* System_String::create(const wchar_t* buff)
{
	static System_String* (*_newUtf16)(const wchar_t* str, int len);
	if (!_newUtf16)
		_newUtf16 = reinterpret_cast<decltype(_newUtf16)>(GetProcAddress(reinterpret_cast<HMODULE>(g_GameAssembly), "il2cpp_string_new_utf16"));

	size_t buffLen = wcslen(buff);
	System_String* str = _newUtf16(buff, buffLen + 1);
	str->_fields._chars[buffLen] = '\0';
	return str;
}


System_String* System_String::concat(System_String* str)
{
	static System_String* (*_concat)(System_String*, System_String*, void*);
	if (!_concat) {
		 Il2CppClass* stringClass = il2cpp::classFromName("mscorlib", "System", "String");
		_concat = reinterpret_cast<decltype(_concat)>(il2cpp::methodFromName(stringClass, "Concat")->methodPointer);
	}

	return _concat(this, str, 0);
}

System_String* System_String::copy()
{
	static System_String* (*_copy)(System_String*, void*);
	if (!_copy) {
		Il2CppClass* stringClass = il2cpp::classFromName("mscorlib", "System", "String");
		_copy = reinterpret_cast<decltype(_copy)>(il2cpp::methodFromName(stringClass, "Copy")->methodPointer);
	}

	return _copy(this, 0);
}

bool System_String::operator==(const System_String& str) const
{
	if (str._fields._strlen != _fields._strlen)
		return false;
	if (wcscmp(str._fields._chars, _fields._chars) == 0)
		return true;
	return false;
}
