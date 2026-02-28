#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT VSYNC
		{
		public:
			static bool* IsFrameLimited;
		};
	}
}