#pragma once

/*
    most things here were taken from
        https://github.com/dreamanlan/il2cpp_ref
*/

#define PUBLIC_KEY_BYTE_LENGTH 8

class Il2CppDomain;
class Il2CppImage;
class Il2CppClass;
class Il2CppType;
class Il2CppGenericInst;
struct ParameterInfo;

typedef struct Il2CppAssemblyName
{
	const char* name;
	const char* culture;
	const char* hash_value;
	const char* public_key;
	unsigned __int32 hash_alg;
	__int32 hash_len;
	unsigned __int32 flags;
	__int32 major;
	__int32 minor;
	__int32 build;
	__int32 revision;
	unsigned __int8 public_key_token[PUBLIC_KEY_BYTE_LENGTH];
} Il2CppAssemblyName;

typedef struct Il2CppAssembly
{
	Il2CppImage* image;
	unsigned __int32 token;
	__int32 referencedAssemblyStart;
	__int32 referencedAssemblyCount;
	Il2CppAssemblyName aname;
} Il2CppAssembly;

struct Il2CppRGCTXData;
struct Il2CppMethodDefinition;
struct Il2CppGenericMethod;
struct Il2CppGenericContainer;

typedef void* MonoMethod;
typedef void (*Il2CppMethodPointer)();
typedef void* (*InvokerMethod)(Il2CppMethodPointer, const MonoMethod*, void*, void**);

typedef struct MethodInfo
{
    Il2CppMethodPointer methodPointer;
    InvokerMethod invoker_method;
    const char* name;
    Il2CppClass* klass;
    const Il2CppType* return_type;
    const ParameterInfo* parameters;

    union
    {
        const Il2CppRGCTXData* rgctx_data; /* is_inflated is true and is_generic is false, i.e. a generic instance method */
        const Il2CppMethodDefinition* methodDefinition;
    };

    /* note, when is_generic == true and is_inflated == true the method represents an uninflated generic method on an inflated type. */
    union
    {
        const Il2CppGenericMethod* genericMethod; /* is_inflated is true */
        const Il2CppGenericContainer* genericContainer; /* is_inflated is false and is_generic is true */
    };

    unsigned __int32 token;
    unsigned __int16 flags;
    unsigned __int16 iflags;
    unsigned __int16 slot;
    unsigned __int8 parameters_count;
    unsigned __int8 is_generic : 1; /* true if method is a generic method definition */
    unsigned __int8 is_inflated : 1; /* true if declaring_type is a generic instance or if method is a generic instance*/
    unsigned __int8 wrapper_type : 1; /* always zero (MONO_WRAPPER_NONE) needed for the debugger */
    unsigned __int8 is_marshaled_from_native : 1; /* a fake MethodInfo wrapping a native function pointer */
} MethodInfo;

typedef struct FieldInfo
{
    const char* name;
    const Il2CppType* type;
    Il2CppClass* parent;
    __int32 offset; // If offset is -1, then it's thread static
    unsigned __int32 token;
} FieldInfo;

template <typename FieldType>
class Il2CppObject {
public:
	class Il2CppClass* _class;
	void* _monitor;
	FieldType _fields;
};
