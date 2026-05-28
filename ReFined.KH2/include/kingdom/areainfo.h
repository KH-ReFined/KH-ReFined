#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		
		class DLL_EXPORT AREAINFO
		{
			public:
				static inline char*(*Get)(int area, int world) = FindSignature<char* (*)(int, int)>("\x44\x8B\xC1\x85\xC9\x79\x08\x44\x0F\xB6\x05", "xxxxxxxxxxx");
		};
	}
}