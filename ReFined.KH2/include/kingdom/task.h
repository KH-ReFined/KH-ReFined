#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include <Windows.h>

extern "C"
{
	extern "C"
	{
		class DLL_EXPORT TASK
		{
			public:
				using sleep_t = void (*)(char* task, int wait);
				static sleep_t sleep;
		};
	}
}