#pragma once

#include "interop.h"

class System_String;

namespace SLZ {
	class Barcode {
	public:
		System_String* toString() {
			static auto fn = reinterpret_cast<System_String * (*)(Barcode*, void*)>(interop::_addresses::Barcode_ToString);
			return fn(this, 0);
		}

	private:
		Barcode();
		~Barcode() = delete;
	};
}
