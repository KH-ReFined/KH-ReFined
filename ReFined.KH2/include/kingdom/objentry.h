#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJENTRY
		{
		public:
			using GetFriendPriority_t = uint32_t(*)(uint16_t id);
			static GetFriendPriority_t GetFriendPriority;

			using Get_t = char* (*)(uint32_t id);
			static Get_t Get;

			using ReadRequestWeapon_t = char* (*)(int part, int hand, uint16_t id, int pri, int bank);
			static ReadRequestWeapon_t ReadRequestWeapon;

			using get_mdlx_t = char* (*)(char* objentryEntry, char* buff);
			static get_mdlx_t get_mdlx;

			using get_apdx_t = char* (*)(char* objentryEntry, char* buff);
			static get_apdx_t get_apdx;

			using get_mset_t = char* (*)(char* objentryEntry, uint32_t objectID, char* buff);
			static get_mset_t get_mset;
		};
	}
}