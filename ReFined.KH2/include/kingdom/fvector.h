#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include <cstdint>

extern "C"
{
	namespace kn
	{
		class DLL_EXPORT FVector
		{
		public:
			static uint32_t* Init(uint32_t* _firstPtr, uint32_t* _secondPtr);
		};
	}
}