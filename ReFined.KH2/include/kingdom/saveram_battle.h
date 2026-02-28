#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SAVERAM_BATTLE
		{
		public:
			using inc_kill_count_t = void(*)(char* saveram_battle, int part);
			static inc_kill_count_t inc_kill_count;
		};
	}
}