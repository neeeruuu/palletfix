#pragma once

template <typename FieldType>
class Il2CppObject {
protected:
	class Il2CppClass* _klass;
	void* _monitor;
	FieldType _fields;
};
