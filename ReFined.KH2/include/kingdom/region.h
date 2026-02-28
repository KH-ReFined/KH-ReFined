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

			static char** Region;
			static char** DefaultRegion;
		};
	}
}