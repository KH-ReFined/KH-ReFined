#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
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