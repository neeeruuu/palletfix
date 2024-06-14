#pragma once

#include "il2cpp/base.h"

namespace il2cpp {
	namespace loader {
		bool populateAssemblies();
	}

	Il2CppClass* classFromName(const char* assemblyName, const char* ns, const char* name);
	MethodInfo* methodFromName(Il2CppClass* ownerClass, const char* name);
	FieldInfo* fieldFromName(Il2CppClass* ownerClass, const char* name);
	Il2CppClass* nestedTypeFromName(Il2CppClass* ownerClass, const char* name);

	void* create(Il2CppClass* objClass);
}