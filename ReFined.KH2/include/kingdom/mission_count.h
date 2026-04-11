#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"
#include "sound.h"
#include "limit.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MISSION_COUNT
		{
		public:
			using destroy_t = void(*)(char* missionCount);
			static destroy_t destroy;
		};
	}
}