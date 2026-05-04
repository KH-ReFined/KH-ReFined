#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT MenuBase
		{
		public:
			static int (*GetMode)();
			static void (*SetSMode)(int mode);
		};
	}
}