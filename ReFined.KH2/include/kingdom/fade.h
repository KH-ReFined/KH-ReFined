#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FADE
		{
		public:
			static inline void (*start)(char* fade, float from, float to, float time) = FindSignature<void(*)(char*, float, float, float)>("\xF3\x0F\x10\x05\x00\x00\x00\x00\x0F\x2F\xC3\x72\x08\xF3\x0F\x10\x1D", "xxxx????xxxxxxxxx");
		};
	}
}