#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	extern "C"
	{
		class DLL_EXPORT TASK
		{
			public:
				static inline void (*sleep)(char* task, int wait) = FetchFunctionFromCall<void(*)(char*, int)>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx", 0x169);
		};
	}
}