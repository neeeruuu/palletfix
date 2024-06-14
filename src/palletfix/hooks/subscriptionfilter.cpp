#include "hooks.h"

#include "util/log.h"
#include "util/memory.h"

#include "il2cpp.h"
#include "il2cpp/version.h"
#include "il2cpp/string.h"

#include <libloaderapi.h>

#include <safetyhook.hpp>

SafetyHookMid verCompHook{};
SafetyHookMid verFixHook{};

void noVersionFix(SafetyHookContext& ctx) {
	System_Version* modVersion = reinterpret_cast<System_Version*>(ctx.rdi);
	if (!modVersion)
		modVersion = System_Version::create();

	modVersion->setMinor(6);
	modVersion->setMajor(0);

	ctx.rdi = reinterpret_cast<uintptr_t>(modVersion);
}

bool applyFilterHooks() {
	Il2CppClass* ModSubscriptionDownloader = il2cpp::classFromName("Assembly-CSharp", "", "ModSubscriptionDownloader");
	if (!ModSubscriptionDownloader)
		return false;

	MethodInfo* FilterSubscriptions = il2cpp::methodFromName(ModSubscriptionDownloader, "FilterSubscriptions");
	if (!FilterSubscriptions)
		return false;

	verFixHook = safetyhook::create_mid(reinterpret_cast<intptr_t>(FilterSubscriptions->methodPointer) + 0x82A, noVersionFix);
	if (!verFixHook.enable())
		return false;

	return true;
}

bool removeFilterHooks() {
	verFixHook.reset();
	return true;
}

HookInfo* _filterHooks = new HookInfo("filter", applyFilterHooks, removeFilterHooks);