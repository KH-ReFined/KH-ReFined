#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SHEET
		{
		public:
			static inline void(*Save)(char* playerSheet) = FindSignature<void(*)(char*)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x8B\x89\x50\x02\x00\x00\xE8\x00\x00\x00\x00\x48\x85\xC0\x0F\x84\x91\x00\x00\x00", "xxxxxxxxxxxxxxxx????xxxxxxxxx");
			static inline void(*Load)(char* playerSheet, uint8_t* partRam) = FindSignature<void(*)(char*, uint8_t*)>("\x40\x53\x57\x48\x83\xEC\x28\x48\x8B\xFA\x48\x8B\xD9\x48\x85\xD2", "xxxxxxxxxxxxxxxx");
		};
	}
}