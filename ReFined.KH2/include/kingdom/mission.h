#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{ 
		class DLL_EXPORT MISSION
		{
		public:
			static inline int(*GetAntiRate)() = FindSignature<int(*)()>("\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x74\x09\x48\x8B\x40\x08\x0F\xB6\x40\x0E\xC3\xB8\x01\x00\x00\x00", "xxx????xxxxxxxxxxxxxxxxxxx");
		};
	}
}
