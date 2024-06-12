#pragma once
class IL2CPPString;

namespace SLZ {
	class Pallet;

	class ModInfo {
	public:
		IL2CPPString* getVersion() {
			return *reinterpret_cast<IL2CPPString**>(reinterpret_cast<__int64>(this) + 0x30);
		}
	private:
		ModInfo();
		~ModInfo() = delete;
	};

	class ModDownloader {
	public:
		ModInfo* getCurrentMod() {
			return *reinterpret_cast<ModInfo**>(reinterpret_cast<__int64>(this) + 0x20);
		}

		Pallet* getCurrentPallet() {
			return *reinterpret_cast<Pallet**>(reinterpret_cast<__int64>(this) + 0x220);
		}

		Pallet* getStagingDir() {
			return *reinterpret_cast<Pallet**>(reinterpret_cast<__int64>(this) + 0x228);
		}
	private:
		ModDownloader();
		~ModDownloader() = delete;
	};
}