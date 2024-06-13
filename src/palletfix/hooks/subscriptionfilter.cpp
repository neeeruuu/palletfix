#include "hooks.h"
#include "signatures.h"

#include "util/log.h"
#include "util/memory.h"

#include "il2cpp/version.h"
#include "il2cpp/string.h"

#include <libloaderapi.h>

#include <safetyhook.hpp>

SafetyHookMid verCompHook{};
SafetyHookMid verFixHook{};

void noVersionFix(SafetyHookContext& ctx) {
	System_Version* modVersion = reinterpret_cast<System_Version*>(ctx.rdi);
	if (modVersion) {
		modVersion->setMinor(6);
		modVersion->setMajor(0);
	}
	else {
		System_String* str = System_String::create(L"0.6", 10 * sizeof(wchar_t));
		ctx.rdi = reinterpret_cast<uintptr_t>(System_Version::fromString(str));
	}
}

bool applyFilterHooks() {
	intptr_t filterSubscriptionsAddr = mem::findPattern(GetModuleHandleA("GameAssembly.dll"), FilterSubscriptions_Signature);
	if (!filterSubscriptionsAddr)
		return false;

	verFixHook = safetyhook::create_mid(filterSubscriptionsAddr + 0x82A, noVersionFix);
	if (!verFixHook.enable())
		return false;

	return true;
}

bool removeFilterHooks() {
	verFixHook.reset();
	return true;
}

HookInfo* _filterHooks = new HookInfo("filter", applyFilterHooks, removeFilterHooks);