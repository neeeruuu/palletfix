#include "interop.h"
#include "signatures.h"

#include <libloaderapi.h>

#include "util/memory.h"

void interop::getAddresses()
{
	void* gameAssembly = LoadLibraryA("GameAssembly.dll");

	_addresses::String_Concat = mem::findPattern(gameAssembly, String_Concat_Signature);
	_addresses::String_Copy = mem::findPattern(gameAssembly, String_Copy_Signature);
	_addresses::String_Create = mem::findPattern(gameAssembly, String_Create_Signature);
	_addresses::Barcode_ToString = mem::findPattern(gameAssembly, Barcode_ToString_Signature);
	_addresses::Marrow_GetRuntimeModsPath = mem::findPattern(gameAssembly, Marrow_getRuntimeModsPath_Signature);
}
