#pragma once

template <typename FieldType>
class Il2CppObject {
protected:
	class Il2CppClass* _klass;
	void* _monitor;
	FieldType _fields;
};

namespace _internal {
	inline void* (*il2cpp_create) (void* typeInfo);
}