#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT PROGRESS
		{
		public:
			static inline bool(*CheckFlag)(int progress) = FindSignature<bool(*)(int)>("\x44\x8B\xC1\x81\xF9\xFF\xFF\x00\x00\x75\x03", "xxxxxxxxxxx");
			static inline void(*SetFlag)(int progress) = FindSignature<void(*)(int)>("\x41\x56\x48\x83\xEC\x40\x48\x89\x5C\x24\x50\x44\x8B\xF1\x48\x89", "xxxxxxxxxxxxxxxx");
		};
	}
}
