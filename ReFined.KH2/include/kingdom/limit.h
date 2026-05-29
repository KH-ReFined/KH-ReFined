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
			static inline void(*destroy)(char* limit_ptr);
		};
	}
}