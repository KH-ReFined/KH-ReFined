#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace dk
	{
		class DLL_EXPORT GAUGE
		{
		public:
			using Init_t = void(*)();
			static Init_t Init;

			static uint64_t pint_playergauge;
			static uint64_t pint_friendgauge;
		};
	}
}