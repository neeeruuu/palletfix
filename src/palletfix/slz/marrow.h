#pragma once

class System_String;

namespace SLZ {
	namespace _internal {
		inline System_String* (*getRuntimeModsPath)(void*);
	}
	
	class MarrowSDK {
	public:
		static System_String* getRuntimeModsPath() {
			return _internal::getRuntimeModsPath(0);
		}
	private:
		MarrowSDK();
		~MarrowSDK() = delete;
	};
}