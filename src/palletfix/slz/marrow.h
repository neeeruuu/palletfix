#pragma once

class System_String;

namespace SLZ {

	class MarrowSDK {
	public:
		static System_String* getRuntimeModsPath();

	private:
		MarrowSDK();
		~MarrowSDK() = delete;
	};
}