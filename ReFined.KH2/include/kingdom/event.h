#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT EVENT
		{
		public:
			static uint64_t pint_eventinfo;
		};
	}
}