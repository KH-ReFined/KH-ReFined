#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SAVERAM_BATTLE
		{
		public:
			static inline void(*inc_kill_count)(char* saveram_battle, int part) = FindSignature<void(*)(char*, int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\xE8\x00\x00\x00\x00\x84\xC0\x74\x42\x8D\x83\x18\xFC\xFF\xFF", "xxxxxxxxxxxxxxxx????xxxxxxxxxx");
		};
	}
} 