#include "palletfix.h"

#include "externs.h"

// from: winnt.h
#define DLL_PROCESS_ATTACH   1
#define DLL_PROCESS_DETACH   0    

int DllMain(void* hMod, unsigned long dwReason, void* /*lpReserved*/) {
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		PalletFix::load(hMod);
		break;
	case DLL_PROCESS_DETACH:
		PalletFix::unload();
		break;
	}
	return 1;
}