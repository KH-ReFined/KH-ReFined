#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT IMAGE_FREEZE
		{
		public:
			static inline char*(*GetImage)(char* imageFreeze, int no) = FindSignature<char* (*)(char*, int)>("\x48\x63\xC2\x48\xC1\xE0\x08\x48\x03\x41\x28\xC3", "xxxxxxxxxxxx");
		};
	}
}