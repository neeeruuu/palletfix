#pragma once

#include "interop.h"

class IL2CPPString;

namespace SLZ {
	class MarrowSDK {
	public:
		static IL2CPPString* getRuntimeModsPath() {
			static auto fn = reinterpret_cast<IL2CPPString*(*)(void*)>(interop::_addresses::Marrow_GetRuntimeModsPath);
			return fn(0);
		}
	private:
		MarrowSDK();
		~MarrowSDK() = delete;
	};
}