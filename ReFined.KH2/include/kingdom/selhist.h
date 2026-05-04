#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT SelHist
		{
		public:
			static void (*Create)(int num, int msg);
		};
	}
}