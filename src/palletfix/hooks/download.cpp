#include "hooks.h"

#include "slz/moddownloader.h"
#include "slz/barcode.h"
#include "slz/marrow.h"
#include "slz/pallet.h"

#include "il2cpp/string.h"
#include "il2cpp.h"

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
		modVersion = System_String::create(L"1.0");
		downloader->getCurrentMod()->setVersion(modVersion);
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

	Il2CppClass* ModDownloader = il2cpp::classFromName("SLZ.Marrow", "SLZ.Marrow.Forklift", "ModDownloader");
	if (!ModDownloader)
		return false;

	/*
		LoadPalletFileAndPathFromZip hook
	*/
	MethodInfo* LoadPalletFileAndPathFromZip = il2cpp::methodFromName(ModDownloader, "<DownloadMod>g__LoadPalletFileAndPathFromZip|17_0");
	if (!LoadPalletFileAndPathFromZip)
		return false;

	// in case something else needs to be changed, 0x52C has:
	// SLZ::Pallet* pallet = reinterpret_cast<SLZ::Pallet*>(ctx.rax);
	// System_String* path = reinterpret_cast<System_String*>(ctx.rdi);
	barcodePathFixHook = safetyhook::create_mid(reinterpret_cast<intptr_t>(LoadPalletFileAndPathFromZip->methodPointer) + 0x556, barcodePathFix);
	if (!barcodePathFixHook.enable())
		return false;

	/*
		DownloadMod hook
	*/
	Il2CppClass* downloadModStateMachine = il2cpp::nestedTypeFromName(ModDownloader, "<DownloadMod>d__17");
	if (!downloadModStateMachine)
		return false;

	MethodInfo* downloadMoveNext = il2cpp::methodFromName(downloadModStateMachine, "MoveNext");
	if (!downloadMoveNext)
		return false;

	verCheckHook = safetyhook::create_mid(reinterpret_cast<intptr_t>(downloadMoveNext->methodPointer) + 0x26E0, palletVersionCheck);
	if (!verCheckHook.enable())
		return false;

	JSONCheckHook = safetyhook::create_mid(reinterpret_cast<intptr_t>(downloadMoveNext->methodPointer) + 0x2E58, palletJSONCheck);
	if (!JSONCheckHook.enable())
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