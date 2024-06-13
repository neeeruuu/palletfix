#pragma once

#include "interop.h"

class System_String;

namespace SLZ {
	class MarrowSDK {
	public:
		static System_String* getRuntimeModsPath() {
			static auto fn = reinterpret_cast<System_String*(*)(void*)>(interop::_addresses::Marrow_GetRuntimeModsPath);
			return fn(0);
		}
	private:
		MarrowSDK();
		~MarrowSDK() = delete;
	};
}