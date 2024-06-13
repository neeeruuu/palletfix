#include "hooks.h"
#include "signatures.h"

#include "slz/moddownloader.h"
#include "slz/barcode.h"
#include "slz/marrow.h"
#include "slz/pallet.h"

#include "il2cpp/string.h"

#include "util/memory.h"

#include <filesystem>

#include <libloaderapi.h>

#include <safetyhook.hpp>

SLZ::ModDownloader* downloader;

void palletVersionCheck(SafetyHookContext& ctx) {
	downloader = reinterpret_cast<SLZ::ModDownloader*>(ctx.rax);

	System_String* palletVersion = downloader->getCurrentPallet()->getVersion();
	System_String* modVersion = downloader->getCurrentMod()->getVersion();

	if (*modVersion != *palletVersion)
		downloader->getCurrentPallet()->setVersion(modVersion->copy());
}

void palletJSONCheck(SafetyHookContext& ctx) {
	System_String* expectedPathStr = reinterpret_cast<System_String*>(ctx.rax);

	std::filesystem::path expectedPath(expectedPathStr->c_str());
	if (std::filesystem::exists(expectedPath))
		return;

	SLZ::Pallet* pallet = downloader->getCurrentPallet();
	System_String* runtimeModsPath = SLZ::MarrowSDK::getRuntimeModsPath();

	std::wstring modPath = std::format(L"{}\\{}", runtimeModsPath->c_str(), pallet->getBarcode()->toString()->c_str());
	std::wstring legacyPalletJsonDir = std::format(L"{}\\pallet.json", modPath);

	if (!std::filesystem::exists(legacyPalletJsonDir))
		return;
	std::filesystem::copy(legacyPalletJsonDir, expectedPathStr->c_str(), std::filesystem::copy_options::overwrite_existing);
}

SafetyHookMid verCheckHook{};
SafetyHookMid JSONChechHook{};

bool applyPalletHooks() {
	void* gameAssembly = LoadLibraryA("GameAssembly.dll");
	if (!gameAssembly)
		return false;

	intptr_t loadAddr = mem::findPattern(gameAssembly, DownloadMod_Signature);
	if (!loadAddr)
		return false;


	verCheckHook = safetyhook::create_mid(loadAddr + 0x26EB, palletVersionCheck);
	if (!verCheckHook.enable())
		return false;

	JSONChechHook = safetyhook::create_mid(loadAddr + 0x2E, palletJSONCheck);
	if (!JSONChechHook.enable())
		return false;

	return true;
}

bool removePalletHooks() {
	verCheckHook.reset();
	JSONChechHook.reset();
	return true;
}

HookInfo* _palletHooks = new HookInfo("pallet", applyPalletHooks, removePalletHooks);