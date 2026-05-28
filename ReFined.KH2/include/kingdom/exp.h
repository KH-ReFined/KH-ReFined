#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Exp
		{
		public:
			static inline int(*getSeqNumber)(char* table, int number, int* outtable) = FindSignature<int(*)(char*, int, int*)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\x05", "xxxxxxxxxxxxxxxxxx");
		};
	}
}