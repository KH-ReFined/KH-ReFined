#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT LIMIT
		{
		public:
			static inline uint32_t(*motion_start)(char* limit_ptr, char* party_ptr, int motion, float blend) = FindSignature<uint32_t(*)(char*, char*, int, float)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\xD9\x0F\x29\x74\x24\x40\x8B\x49\x08\x0F\x28\xF3\x41\x8B\xF8\x48\x8B\xF2", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*_LIMIT)(char* limit_ptr, char* limit_table, char* player, float time) = FindSignature<void(*)(char*, char*, char*, float)>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x56\x41\x57\x48\x83\xEC\x40\x48\x8B\xF9", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*exec)(char* limit_ptr, bool is_first) = FindSignature<void(*)(char*, bool)>("\x48\x89\x5C\x24\x18\x57\x48\x83\xEC\x30\x8B\x41\x54\x0F\xB6\xFA\xC1\xE8\x02\x48\x8B\xD9\xA8\x01\x74\x77", "xxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*destroy)(char* limit_ptr);

			static inline char** Limit = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x30\x48\x83\x3D", "xxxxxxxxxxxxx", 0x34);
		};
	}
}