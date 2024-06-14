#include "il2cpp.h"

#include "globals.h"

#include <filesystem>

#include <libloaderapi.h>

#include <thread>
#include <chrono>
#include <vector>
#include <unordered_map>

#define IL2CPP_FUNCTION(name, signature) \
	static auto name = reinterpret_cast<signature>(GetProcAddress(reinterpret_cast<HMODULE>(g_GameAssembly), #name));\
	if (!name) return 0;

std::unordered_map<std::string, Il2CppAssembly*> assemblies;
Il2CppDomain* domain;

std::vector <std::string> requiredAssemblies{
	"Assembly-CSharp",
	"SLZ.Marrow",
	"SLZ.ModIO"
};

bool il2cpp::loader::populateAssemblies()
{
	IL2CPP_FUNCTION(il2cpp_domain_get, Il2CppDomain * (*)());
	IL2CPP_FUNCTION(il2cpp_domain_get_assemblies, Il2CppAssembly * *(*)(Il2CppDomain*, size_t*));
	IL2CPP_FUNCTION(il2cpp_get_corlib, Il2CppImage * (*)());

	// await il2cpp_init so il2cpp_domain_get call doesn't break
	while (true) {
		if (il2cpp_get_corlib())
			break;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	domain = il2cpp_domain_get();

	Il2CppAssembly** assembliesPtr;
	size_t assemblyCount = 0;
	while (true) {
		size_t prevAssemblyCount = assemblyCount;
		assembliesPtr = il2cpp_domain_get_assemblies(domain, &assemblyCount);

		for (int i = 0; i < assemblyCount; i++) {
			Il2CppAssembly* currentAssembly = assembliesPtr[i];
			auto it = std::find(requiredAssemblies.begin(), requiredAssemblies.end(), currentAssembly->aname.name);
			if (it != requiredAssemblies.end())
				requiredAssemblies.erase(it);
		}

		if (requiredAssemblies.size() == 0)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	for (int i = 0; i < assemblyCount; i++) {
		auto assembly = assembliesPtr[i];
		assemblies[assembly->aname.name] = assembly;
	}

	return true;
}

Il2CppClass* il2cpp::classFromName(const char* assemblyName, const char* ns, const char* name)
{
	IL2CPP_FUNCTION(il2cpp_class_from_name, Il2CppClass * (*)(Il2CppImage*, const char*, const char*));
	Il2CppAssembly* assembly = assemblies.at(assemblyName);
	if (!assembly) return nullptr;
	return il2cpp_class_from_name(assembly->image, ns, name);
}

MethodInfo* il2cpp::methodFromName(Il2CppClass* ownerClass, const char* name)
{
	IL2CPP_FUNCTION(il2cpp_class_get_methods, MethodInfo * (*)(Il2CppClass*, void** iter));
	IL2CPP_FUNCTION(il2cpp_method_get_name, const char* (*)(MethodInfo*));

	void* iter = nullptr;
	MethodInfo* method = nullptr;
	const char* methodName = nullptr;
	while (true) {
		method = il2cpp_class_get_methods(ownerClass, &iter);
		if (!method || !iter)
			break;

		methodName = il2cpp_method_get_name(method);
		if (!stricmp(name, methodName))
			return method;
	}

	return nullptr;
}

FieldInfo* il2cpp::fieldFromName(Il2CppClass* ownerClass, const char* name)
{
	IL2CPP_FUNCTION(il2cpp_class_get_fields, FieldInfo * (*)(Il2CppClass*, void** iter));
	IL2CPP_FUNCTION(il2cpp_field_get_name, const char* (*)(FieldInfo*));

	void* iter = nullptr;
	FieldInfo* field = nullptr;
	const char* fieldName = nullptr;
	while (true) {
		field = il2cpp_class_get_fields(ownerClass, &iter);
		if (!field || !iter)
			break;

		fieldName = il2cpp_field_get_name(field);
		if (!stricmp(name, fieldName))
			return field;
	}

	return nullptr;
}

Il2CppClass* il2cpp::nestedTypeFromName(Il2CppClass* ownerClass, const char* name)
{
	IL2CPP_FUNCTION(il2cpp_class_get_nested_types, Il2CppClass * (*)(Il2CppClass*, void** iter));
	IL2CPP_FUNCTION(il2cpp_class_get_name, const char* (*)(Il2CppClass*));

	void* iter = nullptr;
	Il2CppClass* nestedClass = nullptr;
	const char* className = nullptr;
	while (true) {
		nestedClass = il2cpp_class_get_nested_types(ownerClass, &iter);
		if (!nestedClass || !iter)
			break;

		className = il2cpp_class_get_name(nestedClass);
		if (!stricmp(className, name))
			return nestedClass;
	}

	return nullptr;
}

void* il2cpp::create(Il2CppClass* objClass)
{
	IL2CPP_FUNCTION(il2cpp_object_new, void*(*)(Il2CppClass*));
	return il2cpp_object_new(objClass);
}
