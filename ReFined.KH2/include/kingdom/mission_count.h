#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MISSION_COUNT
		{
			public:
				static void(*destroy)(char* missionCount);
		};
	}
}