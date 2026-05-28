#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

#include <cstdint>
#include <Windows.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COMMAND_MENU
		{
		public:

			static inline char* (*add)(char* commandMenu, int command, char* target, int amount) = FindSignature<char* (*)(char*, int, char*, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x30\x48\x8B\xD9\x41\x8B\xE9\x8B\xCA\x4D\x8B\xF0\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline char* (*make_by_table)(char* commandMenu, char* table, size_t size, char* target) = FindSignature<char* (*)(char*, char*, size_t, char*)>("\x48\x8B\xC4\x4C\x89\x48\x20\x48\x89\x50\x10\x53\x41\x54\x41\x57\x48\x83\xEC\x50\x45\x33\xFF\xC7\x41\x10\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}