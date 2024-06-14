#include "marrow.h"
#include "il2cpp.h"

System_String* SLZ::MarrowSDK::getRuntimeModsPath()
{
	static System_String* (*_getRuntimeModsPath)(void*);
	if (!_getRuntimeModsPath) {
		Il2CppClass* sdkClass = il2cpp::classFromName("SLZ.Marrow", "SLZ.Marrow", "MarrowSDK");
		_getRuntimeModsPath = reinterpret_cast<decltype(_getRuntimeModsPath)>(il2cpp::methodFromName(sdkClass, "get_RuntimeModsPath")->methodPointer);
	}
	return _getRuntimeModsPath(0);
}
