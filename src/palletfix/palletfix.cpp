#include "palletfix.h"
#include "interop.h"
#include "hooks.h"

#include "util/log.h"

#include <libloaderapi.h>

void PalletFix::load(void* dllMod)
{
#ifdef _DEBUG
	Log::createConsole();
	Log::setupConsole();
#endif

	Log::info("init");

	interop::getAddresses();

	if (!HookManager::get()->applyAll()) {
		Log::error("failed to apply hooks, unloading...");
		FreeLibrary(reinterpret_cast<HMODULE>(dllMod));
		return;
	}
}

void PalletFix::unload()
{
	if (!HookManager::get()->removeAll())
		Log::warning("failed to remove all hooks");
}
