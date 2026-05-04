#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT CMenuHelp
		{
		public:
			static void (*Create)(char* message, bool param_xy);
		};
	}
}