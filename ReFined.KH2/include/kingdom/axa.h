#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Axa
	{
		class DLL_EXPORT ResourceEntry
		{
			public:
				static inline char*(*ReadDataLocal)(char* resourceEntry, char* base, const char* folder, const char* file) = FindSignature<char* (*)(char*, char*, const char*, const char*)>("\x4C\x8B\x91\x70\x06\x00\x00\x4D\x8B\xD9\x4D\x85\xD2", "xxxxxxxxxxxxx");
		};
	}
}