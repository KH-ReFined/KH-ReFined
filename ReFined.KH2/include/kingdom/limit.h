#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"
#include "sound.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT LIMIT
		{
		public:
			using motion_start_t = uint32_t(*)(char* limit_ptr, char* party_ptr, int motion, float blend);
			static motion_start_t motion_start;
		};
	}
}