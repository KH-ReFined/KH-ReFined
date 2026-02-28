#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT WEAPON
		{
		public:
			using Init_t = char* (*)(char* addr, char* objentry, char* partyObject, int part, int hand, int priority);
			static Init_t Init;
		};
	}
}