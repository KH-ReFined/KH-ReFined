#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Vsync
		{
		public:
			static void(*setLimit)(int limit);

			static bool* IsFrameLimited;
			static float* GameSpeed;
		};
	}
}