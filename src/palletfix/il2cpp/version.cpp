#include "version.h"
#include "il2cpp.h"

void System_Version::operator=(const System_Version& ver)
{
	_fields._major = ver._fields._major;
	_fields._minor = ver._fields._minor;
	_fields._build = ver._fields._build;
	_fields._revision = ver._fields._revision;
}

System_Version* System_Version::create()
{
	static auto verClass = il2cpp::classFromName("mscorlib", "System", "Version");
	return reinterpret_cast<System_Version*>(il2cpp::create(verClass));
}

System_Version* System_Version::create(System_String* str)
{
	static System_Version* (*_fromString) (System_String*, void*);
	if (!_fromString) {
		Il2CppClass* envClass = il2cpp::classFromName("mscorlib", "System", "Environment");
		_fromString = reinterpret_cast<decltype(_fromString)>(il2cpp::methodFromName(envClass, "CreateVersionFromString")->methodPointer);
	}

	return _fromString(str, 0);
}