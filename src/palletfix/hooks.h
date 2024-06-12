#pragma once

#include <string>
#include <vector>
#include <mutex>

//#define HOOK(handle, name, sig)\
//LPVOID lp##name = reinterpret_cast<LPVOID>(mem::findPattern(handle, sig));\
//if (!lp##name)\
//	return false;\
//if (MH_CreateHook(lp##name, &h##name, reinterpret_cast<LPVOID*>(&o##name)) != MH_OK) {\
//	Log::error("Failed to create hook for {}", #name);\
//	return false;\
//}\
//MH_EnableHook(lp##name);


typedef bool (*tHookCallback) ();

class HookInfo;

class HookManager {
	friend HookInfo;
public:
	static HookManager* get();

	bool applyAll();
	bool removeAll();

protected:
	void registerHook(HookInfo* hook);
	void unregisterHook(HookInfo* hook);

private:
	std::vector<HookInfo*> _hooks;
	static HookManager* _instance;
	mutable std::mutex _mutex;
};

class HookInfo {
	friend HookManager;
public:
	HookInfo(std::string name, tHookCallback apply, tHookCallback remove);
	~HookInfo();

protected:
	__forceinline bool apply() const { return _apply(); }
	__forceinline bool remove() const { return _remove(); }
	__forceinline const std::string getName() const { return _name; }
private:
	HookInfo();

	std::string _name;
	tHookCallback _apply;
	tHookCallback _remove;
};