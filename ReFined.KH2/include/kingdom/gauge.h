#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
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

			static char** PlayerGauge;
			static char** FriendGauge;
		};
	}
}