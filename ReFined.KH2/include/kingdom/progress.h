#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT PROGRESS
		{
		public:
			using CheckFlag_t = bool(*)(int progress);
			static CheckFlag_t CheckFlag;
		};
	}
}
