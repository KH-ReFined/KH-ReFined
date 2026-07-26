#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT CmTutorial
		{
		public:
			static inline void (*Setup)(int type, int mode) = FindSignature<void(*)(int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x8B\xF1\x8B\xDA\xB1\x01", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}