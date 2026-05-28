#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT WEAPON_MSET
		{ 
		public:
			static inline char* (*GetFilename)(int part, int hand, char* buff) = FindSignature<char*(*)(int, int, char*)>("\x48\x83\xEC\x28\x48\x63\xC2\x4C\x63\xC9\x4A\x8D\x14\x48\x48\xC1", "xxxxxxxxxxxxxxxx");
		};
	}
}