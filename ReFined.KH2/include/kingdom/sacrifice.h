#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SACRIFICE
		{
		public:
			using GetFormStatus_t = int(*)(int form);
			static GetFormStatus_t GetFormStatus;
		};
	}
}