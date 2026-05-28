#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	class DLL_EXPORT WORLD
	{
	public:
		static inline char* (*GetName)(int world) = FetchFunctionFromCall<char*(*)(int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xD1\x83\xEA\x64\x74", "xxxxxxxxxxxxxxxx", 0x1B);
	};
}