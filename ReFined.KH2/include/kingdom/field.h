#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "SigScan.h"

#include <cstdint>
#include <Windows.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT  FIELD
		{
		public:

			using TASK_FUNC = void(*)(char*);
			static char* TaskManager;

			using CreateThread_t = void(*)(int id, int priority, TASK_FUNC entryFunc, int stackSize);
			static CreateThread_t CreateThread;

			static bool TaskExists(uint64_t taskFunction);
		};
	}
}