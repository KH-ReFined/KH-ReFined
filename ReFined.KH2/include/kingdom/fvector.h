#pragma once
#include <cstdint>

#define DLL_EXPORT __declspec(dllexport)

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