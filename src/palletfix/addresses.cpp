#include "addresses.h"
#include "signatures.h"
#include "globals.h"

#include <libloaderapi.h>

#include "util/memory.h"
#include "util/log.h"

#include "slz/barcode.h"
#include "slz/marrow.h"

#include "il2cpp/base.h"
#include "il2cpp/string.h"
#include "il2cpp/version.h"

#define APPLY_ADDR(dest, name)\
	if (!addr) return false;\
	Log::verbose("{} - {}", name, reinterpret_cast<void*>(addr));\
	dest = reinterpret_cast<decltype(dest)>(addr);\
	addr = 0;


#define GET_SIG(dest, sig) addr = mem::findPattern(g_GameAssembly, sig);\
	APPLY_ADDR(dest, #dest)\

#define GET_PROCADDR(dest, name)\
	addr = reinterpret_cast<intptr_t>(GetProcAddress(reinterpret_cast<HMODULE>(g_GameAssembly), name)); \
	APPLY_ADDR(dest, name);

bool addresses::get()
{
	g_GameAssembly = LoadLibraryA("GameAssembly.dll");
	if (!g_GameAssembly)
		return false;

	intptr_t addr;
	GET_SIG(_internal::String__Concat, String_Concat_Signature);
	GET_SIG(_internal::String__Copy, String_Copy_Signature);
	GET_PROCADDR(_internal::String__New, "il2cpp_string_new");
	GET_PROCADDR(_internal::String__New_UTF16, "il2cpp_string_new_utf16");

	GET_SIG(_internal::Version__FromString, Version_FromString_Signature);

	GET_SIG(SLZ::_internal::Barcode__ToString, Barcode_ToString_Signature);
	GET_SIG(SLZ::_internal::getRuntimeModsPath, Marrow_getRuntimeModsPath_Signature);

	GET_SIG(_internal::il2cpp_create, Il2Cpp_Create_Signature);



	return true;
}
