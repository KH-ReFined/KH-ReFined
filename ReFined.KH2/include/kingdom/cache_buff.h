#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT CACHE_BUFF
		{
		public:
			using SearchByName_t = char* (*)(const char* fileName, int pri);
			static SearchByName_t SearchByName;

			using DestroyPriority_t = void(*)(uint32_t priority);
			static DestroyPriority_t DestroyPriority;

			using Flush_t = void(*)(char* task);
			static Flush_t Flush;

			using IsFlushing_t = bool(*)();
			static IsFlushing_t IsFlushing;

			using GetStatus_t = int(*)(const char* name);
			static GetStatus_t GetStatus;
		};
	}
}