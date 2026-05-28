#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT MenuUtil
		{
		public:
			static inline char* (*CreateSprt)(char* spriteMessage, int priority, uint32_t s_unit, uint32_t i_unit, char* layout, int fin, uint32_t loop, int group, int offset16x9) = FindSignature<char*(*)(char*, int, uint32_t, uint32_t, char*, int, uint32_t, int, int)>("\x48\x89\x6C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x40\x4C\x8B\xF1\x41\x8B\xF1\x48\x8B\x4C\x24\x70", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline char* (*CreateMess)(char* spriteMessage, int priority, uint32_t s_unit, uint32_t i_unit, char* layout, int fin, uint32_t loop, int messageNo, int group, int offset16x9) = FindSignature<char*(*)(char*, int, uint32_t, uint32_t, char*, int, uint32_t, int, int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x40\x48\x8B\xE9\x41\x8B\xF1", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*SetSprtParent)(char* target, char* parent) = FindSignature<void(*)(char*, char*)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x89\x91\xE0\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x83\xBB\xE0\x01\x00\x00\x00\x74\x0A\x33\xD2\x48", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxx");
		};
	}
}