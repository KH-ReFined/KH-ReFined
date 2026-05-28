#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT CMenuHelp
		{
			public:
				static inline void (*Create)(char* message, bool param_xy) = FindSignature<void(*)(char*, bool)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x50\x48\x8B\xD9\x0F\xB6\xFA\x48\x8B\x0D", "xxxxxxxxxxxxxxxxxxx");
		};
	}
}