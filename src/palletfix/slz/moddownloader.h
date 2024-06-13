#pragma once
class System_String;

namespace SLZ {
	class Pallet;

	class ModInfo {
	public:
		System_String* getVersion() {
			return *reinterpret_cast<System_String**>(reinterpret_cast<__int64>(this) + 0x30);
		}

		void setVersion(System_String* ver) {
			*reinterpret_cast<System_String**>(reinterpret_cast<__int64>(this) + 0x30) = ver;
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