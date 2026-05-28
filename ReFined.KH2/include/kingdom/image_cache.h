#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT IMAGE_CACHE
		{
		public:
			static inline char*(*Load)(char* imageBuff) = FindSignature<char* (*)(char*)>("\x48\x89\x5C\x24\x18\x48\x89\x6C\x24\x20\x41\x56\x48\x83\xEC\x20\x44\x8B\x89\xF4\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}