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

	namespace kn
	{
		class DLL_EXPORT Friend
		{
		public:
			using equippedAbility_t = bool(*)(char* obj, int abilityID, int abilityItemID);
			static equippedAbility_t equippedAbility;
		};
	}
}