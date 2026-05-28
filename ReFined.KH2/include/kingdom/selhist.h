#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT SelHist
		{
		public:
			static inline void (*Create)(int num, int msg) = FindSignature<void(*)(int, int)>("\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x40\x48\x8B\x2D", "xxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}