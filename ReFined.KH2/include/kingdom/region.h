#pragma once

#include <cstdint>
#include <SigScan.h>

#define DLL_EXPORT __declspec(dllexport)

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT REGION
		{
		public:
			using Get_t = uint32_t(*)();
			static Get_t Get;

			static uint64_t* pint_region;
			static uint64_t* pint_region_default;
		};
	}
}