#pragma once

class IL2CPPString;

namespace SLZ {
	class Barcode;

	class Pallet {
	public:
		Barcode* getBarcode() {
			return *reinterpret_cast<Barcode**>(reinterpret_cast<__int64>(this) + 0x18);
		}

		IL2CPPString* getName() {
			return *reinterpret_cast<IL2CPPString**>(reinterpret_cast<__int64>(this) + 0x30);
		}

		IL2CPPString* getVersion() {
			return *reinterpret_cast<IL2CPPString**>(reinterpret_cast<__int64>(this) + 0x58);
		}

		void setVersion(IL2CPPString* ver) {
			*reinterpret_cast<void**>(reinterpret_cast<__int64>(this) + 0x58) = ver;
		}

	private:
		Pallet();
		~Pallet() = delete;
	};
}