#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COLLISION_DATA
		{
			public:
				static inline char* (*next_elem)(char* colldata, int type, char* elem) = FetchFunctionFromCall<char* (*)(char*, int, char*)>("\x4D\x85\xC0\x4D\x8D\x48\x14\x4C\x63\x01\x48\x8D\x41\x40\x49\x0F\x45\xC1\x4F\x8D\x04\x80\x4D\x8D\x40\x10\x4E\x8D\x04\x81\x49\x3B\xC0\x73\x16", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}