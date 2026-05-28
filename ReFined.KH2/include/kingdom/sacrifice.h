#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SACRIFICE
		{
		public:
			static inline int(*GetFormStatus)(int form) = FindSignature<int(*)(int)>("\x40\x56\x57\x41\x56\x48\x83\xEC\x20\x8B\xF9\x45\x33\xF6\x33\xF6", "xxxxxxxxxxxxxxxx");
		};
	}
}