#include "barcode.h"
#include "il2cpp.h"

System_String* SLZ::Barcode::toString()
{
	static System_String* (*_toString)(Barcode*, void*);
	if (!_toString) {
		Il2CppClass* barcodeClass = il2cpp::classFromName("SLZ.Marrow", "SLZ.Marrow.Warehouse", "Barcode");
		_toString = reinterpret_cast<decltype(_toString)>(il2cpp::methodFromName(barcodeClass, "ToString")->methodPointer);
	}
	return _toString(this, 0);
}
