#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FRIEND
		{
		public:
			static uint64_t pint_friend;
		};
	}
}