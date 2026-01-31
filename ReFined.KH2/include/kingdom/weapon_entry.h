#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT WEAPON_ENTRY
		{
		public:
			using Get_t = uint16_t(*)(int part, uint16_t item);
			static Get_t Get;
		};
	}
}