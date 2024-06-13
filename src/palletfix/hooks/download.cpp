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


	if (!modVersion) {
		System_String* verStr = System_String::create(L"1.0");
		downloader->getCurrentMod()->setVersion(verStr);
	}
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

void barcodePathFix(SafetyHookContext& ctx) {
	ctx.r8 = (ctx.r8 & 0xFFFFFFFF00000000) | (intptr_t)3;
}

SafetyHookMid verCheckHook{};
SafetyHookMid JSONCheckHook{};

SafetyHookMid barcodePathFixHook{};

bool applyPalletHooks() {
	void* gameAssembly = LoadLibraryA("GameAssembly.dll");
	if (!gameAssembly)
		return false;


	intptr_t loadAddr = mem::findPattern(gameAssembly, LoadPalletFileAndPathFromZip_Signature);
	if (!loadAddr) return false;
	
	intptr_t loadAddr2 = mem::findPattern(gameAssembly, LoadPalletFileAndPathFromZip2_Signature);
	if (!loadAddr2) return false;

	verCheckHook = safetyhook::create_mid(loadAddr + 0x26E0, palletVersionCheck);
	if (!verCheckHook.enable())
		return false;

	JSONCheckHook = safetyhook::create_mid(loadAddr + 0x2E58, palletJSONCheck);
	if (!JSONCheckHook.enable())
		return false;

	// 0x52C has:
	// SLZ::Pallet* pallet = reinterpret_cast<SLZ::Pallet*>(ctx.rax);
	// System_String* path = reinterpret_cast<System_String*>(ctx.rdi);
	barcodePathFixHook = safetyhook::create_mid(loadAddr2 + 0x556, barcodePathFix);
	if (!barcodePathFixHook.enable())
		return false;

	return true;
}

bool removePalletHooks() {
	verCheckHook.reset();
	JSONCheckHook.reset();
	barcodePathFixHook.reset();
	return true;
}

HookInfo* _palletHooks = new HookInfo("pallet", applyPalletHooks, removePalletHooks);