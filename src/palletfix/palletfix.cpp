#include "palletfix.h"
#include "hooks.h"
#include "il2cpp.h"
#include "globals.h"

#include "util/log.h"

#include <libloaderapi.h>
#include <shlobj.h>
#include <comdef.h>

#include <regex>
#include <string>
#include <format>
#include <memory>
#include <fstream>
#include <filesystem>

static std::regex modSettingsPattern(R"("mod\.io\.invalid_mods"\s*:\s*\{([^}]*)\})");
void checkFirstTimeSetup() {
	PWSTR localLow = NULL;

	if (SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &localLow) < 0)
		return;

	std::shared_ptr<void> llCleanup(localLow, [&](void*) { CoTaskMemFree(localLow); }); // yes i couldn't be arsed to type this for every return

	std::wstring bonelabPath = std::format(L"{}\\Stress Level Zero\\BONELAB", localLow);
	std::wstring palletFirstTimeCheck = std::format(L"{}\\palfix", bonelabPath);

	if (std::filesystem::exists(palletFirstTimeCheck))
		return;

	std::wstring modSettingsPath = std::format(L"{}\\mod_settings.json", bonelabPath);
	std::ifstream modSettingsStream(modSettingsPath);

	if (!modSettingsStream.is_open())
		return;

	std::string modSettings((std::istreambuf_iterator<char>(modSettingsStream)), std::istreambuf_iterator<char>());
	modSettingsStream.close();
	modSettings = std::regex_replace(modSettings, modSettingsPattern, "invalid_mods\":{}");

	std::ofstream modSettingsOut(modSettingsPath);
	if (!modSettingsOut.is_open())
		return;

	modSettingsOut << modSettings;
	modSettingsOut.close();

	std::ofstream firstTimeIdentifier(palletFirstTimeCheck);
	if (!firstTimeIdentifier.is_open())
		return;
	
	firstTimeIdentifier << "This file exists only so that PalletFix doesn't clear invalid_mods on every load.";
	firstTimeIdentifier.close();
}

void PalletFix::load(void* dllMod)
{
#ifdef _DEBUG
	Log::createConsole();
	Log::setupConsole();
#endif

	Log::info("loading");

	checkFirstTimeSetup();

	g_GameAssembly = LoadLibraryA("GameAssembly.dll");
	if (!g_GameAssembly) {
		Log::error("failed to get GameAssembly handle, unloading...");
		FreeLibrary(reinterpret_cast<HMODULE>(dllMod));
		return;
	}

	Log::info("getting il2cpp assemblies");
	if (!il2cpp::loader::populateAssemblies()) {
		Log::error("failed to get il2cpp assemblies");
		FreeLibrary(reinterpret_cast<HMODULE>(dllMod));
		return;
	}

	Log::info("applying hooks");
	if (!HookManager::get()->applyAll()) {
		Log::error("failed to apply hooks, unloading...");
		FreeLibrary(reinterpret_cast<HMODULE>(dllMod));
		return;
	}
}

void PalletFix::unload()
{
	Log::info("unloading");

	if (!HookManager::get()->removeAll())
		Log::warning("failed to remove all hooks");
}
