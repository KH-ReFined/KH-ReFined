#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SORA
		{
		public:
			using AddHP_t = void(*)(char* playerObject, int value, int parts, bool is_voice);
			static AddHP_t AddHP;

			using RefreshAbilities_t = void(*)(char* playerStats);
			static RefreshAbilities_t RefreshAbilities;

			using GetEntryID_t = uint16_t(*)(uint32_t form);
			static GetEntryID_t GetEntryID;

			using IsChanging_t = bool(*)(char* playerObject);
			static IsChanging_t IsChanging;

			static uint64_t pint_sora;
		};
	}
}