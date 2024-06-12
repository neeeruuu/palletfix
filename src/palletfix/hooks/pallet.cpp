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

#include <Zydis/Zydis.h>
#include <safetyhook.hpp>

SLZ::ModDownloader* downloader;

void midHookOne(SafetyHookContext& ctx) {
	downloader = reinterpret_cast<SLZ::ModDownloader*>(ctx.rax);

	IL2CPPString* palletVersion = downloader->getCurrentPallet()->getVersion();
	IL2CPPString* modVersion = downloader->getCurrentMod()->getVersion();

	if (*modVersion != *palletVersion)
		downloader->getCurrentPallet()->setVersion(modVersion->copy());
}

void midHookTwo(SafetyHookContext& ctx) {
	IL2CPPString* expectedPathStr = reinterpret_cast<IL2CPPString*>(ctx.rax);

	std::filesystem::path expectedPath(expectedPathStr->c_str());
	if (std::filesystem::exists(expectedPath))
		return;

	SLZ::Pallet* pallet = downloader->getCurrentPallet();
	IL2CPPString* runtimeModsPath = SLZ::MarrowSDK::getRuntimeModsPath();

	std::wstring modPath = std::format(L"{}\\{}", runtimeModsPath->c_str(), pallet->getBarcode()->toString()->c_str());
	std::wstring legacyPalletJsonDir = std::format(L"{}\\pallet.json", modPath);

	if (!std::filesystem::exists(legacyPalletJsonDir))
		return;
	std::filesystem::copy(legacyPalletJsonDir, expectedPathStr->c_str(), std::filesystem::copy_options::overwrite_existing);
}

SafetyHookMid mid1{};
SafetyHookMid mid2{};

bool applyPalletHooks() {
	void* gameAssembly = LoadLibraryA("GameAssembly.dll");
	if (!gameAssembly)
		return false;

	intptr_t loadAddr = mem::findPattern(gameAssembly, DownloadMod_Signature);
	if (!loadAddr)
		return false;

	intptr_t midAddr1 = loadAddr + 0x26EB;
	intptr_t midAddr2 = loadAddr + 0x2E58;

	mid1 = safetyhook::create_mid(midAddr1, midHookOne);
	if (!mid1.enable())
		return false;

	mid2 = safetyhook::create_mid(midAddr2, midHookTwo);
	if (!mid2.enable())
		return false;

	return true;
}

bool removePalletHooks() {
	mid1.reset();
	mid2.reset();
	return true;
}

HookInfo* _palletHooks = new HookInfo("pallet", applyPalletHooks, removePalletHooks);