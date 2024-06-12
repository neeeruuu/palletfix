#pragma once

#include "interop.h"

class IL2CPPString;

namespace SLZ {
	class Barcode {
	public:
		IL2CPPString* toString() {
			static auto fn = reinterpret_cast<IL2CPPString * (*)(Barcode*, void*)>(interop::_addresses::Barcode_ToString);
			return fn(this, 0);
		}

	private:
		Barcode();
		~Barcode() = delete;
	};
}
