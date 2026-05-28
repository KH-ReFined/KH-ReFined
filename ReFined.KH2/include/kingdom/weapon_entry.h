#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT WEAPON_ENTRY
		{
		public:
			static inline uint16_t(*Get)(int part, uint16_t item) = FetchFunctionFromCall<uint16_t(*)(int, uint16_t)>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x152);

		};
	}
}