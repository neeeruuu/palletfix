#pragma once

class System_String;

namespace SLZ {
	class Barcode;

	class Pallet {
	public:
		Barcode* getBarcode() {
			return *reinterpret_cast<Barcode**>(reinterpret_cast<__int64>(this) + 0x18);
		}

		System_String* getName() {
			return *reinterpret_cast<System_String**>(reinterpret_cast<__int64>(this) + 0x30);
		}

		System_String* getVersion() {
			return *reinterpret_cast<System_String**>(reinterpret_cast<__int64>(this) + 0x58);
		}

		void setVersion(System_String* ver) {
			*reinterpret_cast<void**>(reinterpret_cast<__int64>(this) + 0x58) = ver;
		}

	private:
		Pallet();
		~Pallet() = delete;
	};
}