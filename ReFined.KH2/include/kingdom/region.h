#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT REGION
		{
		public:
			using Get_t = uint32_t(*)();
			static Get_t Get;

			static char** Region;
			static char** DefaultRegion;
		};
	}
}