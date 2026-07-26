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
				static inline void (*FadeOut)() = FetchFunctionFromCall<void(*)()>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xF8\xE8\x00\x00\x00\x00\x8B\xF0\x83\xF8\x02\x0F\x85\xBD\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????x????xxx????xxxxxxxxxxx", 0x1EE);
		};
	}
}