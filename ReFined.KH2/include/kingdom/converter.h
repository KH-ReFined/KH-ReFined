#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace PC
	{
		class DLL_EXPORT CONVERTER
		{
		public:
			using LONG_TO_INT_ADDRESS_t = uint32_t(*)(uint64_t);
			static LONG_TO_INT_ADDRESS_t LONG_TO_INT_ADDRESS;

			using INT_TO_LONG_ADDRESS_t = uint64_t(*)(uint32_t);
			static INT_TO_LONG_ADDRESS_t INT_TO_LONG_ADDRESS;
		};
	}
}