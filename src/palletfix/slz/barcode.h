#pragma once

class System_String;

namespace SLZ {
	class Barcode;
	
	namespace _internal {
		inline System_String* (*Barcode__ToString) (Barcode*, void*);
	};

	class Barcode {
	public:
		System_String* toString() {
			return _internal::Barcode__ToString(this, 0);
		}

	private:
		Barcode();
		~Barcode() = delete;
	};
}
