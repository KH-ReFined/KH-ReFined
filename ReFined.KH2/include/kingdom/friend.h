#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FRIEND
		{
		public:
			static inline char** Friend = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x30\x4D\x8B\xC8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x5E);
			static inline char* (*AppearNearPlayer)(short id, char* orig_pos, float height, float near_range, float far_range) = FindSignature<char* (*)(short, char*, float, float, float)>("\x48\x8B\xC4\x48\x89\x58\x10\x55\x48\x8D\x68\xA9\x48\x81\xEC\xA0\x00\x00\x00\x0F\x29\x70\xE8\x0F\x28\xF2\x0F\x29\x78\xD8\x0F\x28", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		};
	}

	namespace kn
	{
		class DLL_EXPORT Friend
		{
		public:
			static inline bool(*equippedAbility)(char* obj, int abilityID, int abilityItemID) = FindSignature<bool(*)(char*, int, int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x41\x8B\xF8\x48\x8B\xD9\x45\x85\xC0\x79\x09\x8B\xCA\xE8", "xxxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}