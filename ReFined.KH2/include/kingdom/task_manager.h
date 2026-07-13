#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	class DLL_EXPORT TASK_MANAGER
	{
	public:
		static inline void(*create_thread)(char* task_manager, int id, int priority, void(*entry_func)(char*)) = FindSignature<void(*)(char*, int, int, void(*)(char*))>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x48\x8B\x01\x41\x8B\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	};
}