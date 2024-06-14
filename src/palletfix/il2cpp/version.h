#include "base.h"

class System_String;

struct System_Version_fields {
	__int32 _major;
	__int32 _minor;
	__int32 _build;
	__int32 _revision;
};

class System_Version : Il2CppObject<System_Version_fields> {
public:
	__inline int getMajor() const { return _fields._major; }
	__inline int getMinor() const { return _fields._minor; }
	__inline int getBuild() const { return _fields._build; }
	__inline int getRevision() const { return _fields._revision; }

	__inline void setMajor(int val) { _fields._major = val; }
	__inline void setMinor(int val) { _fields._minor = val; }
	__inline void setBuild(int val) { _fields._build = val; }
	__inline void setRevision(int val) { _fields._revision = val; }

	void operator=(const System_Version& ver);

	static System_Version* create();
	static System_Version* create(System_String* str);
};