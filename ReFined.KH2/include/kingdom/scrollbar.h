#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT ScrollBar
		{
		public:
			static inline void(*Create)(char* scrollBar, int pri, char* sqd, char* img, char* seq, int max, int dips, char* parent) = FindSignature<void(*)(char*, int, char*, char*, char*, int, int, char*)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x50\x48\x8B\xF1\x0F\x29\x74\x24\x40\x48\x8B\x09\x49", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*SetPos)(char* scrollBar, int pos) = FindSignature<void(*)(char*, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x48\x8B\xD9\x89", "xxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*SetMax)(char* scrollBar, int max) = FindSignature<void(*)(char*, int)>("\x40\x53\x48\x83\xEC\x30\x44\x8B\x41\x2C\x8B\xC2\xF3\x0F\x10\x15", "xxxxxxxxxxxxxxxx");
		};
	}
}