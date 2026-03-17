#pragma once
#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MISSION
		{
		public:
			using GetAntiRate_t = int(*)();
			static GetAntiRate_t GetAntiRate;
		};
	}
}
