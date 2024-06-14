#pragma once


class System_String;

namespace SLZ {
	class Barcode {
	public:
		System_String* toString();

	private:
		Barcode();
		~Barcode() = delete;
	};
}
