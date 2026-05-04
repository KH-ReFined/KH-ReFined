#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "sora.h"
#include "friend.h"
#include "converter.h"
#include "member.h"
#include "weapon_entry.h"
#include "weapon_mset.h"
#include "weapon.h"
#include "objentry.h"
#include "panacea_alloc.h"
#include "sheet.h"
#include "cache_buff.h"
#include "file.h"
#include "pax.h"
#include "area.h"
#include "field.h"
#include "item.h"
#include "sound.h"
#include "memorymgr.h"

#include <thread>
#include <cassert>
#include <member_table.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT PARTY
		{
		public:
			using SetWeapon_t = void(*)(char* party_ptr, int entry_id, int hand);
			static SetWeapon_t SetWeapon;

			using ExecuteCommand_t = void(*)(char* playerObject, char* command);
			static ExecuteCommand_t ExecuteCommand;

			using get_partram_t = char* (*)(char* party_ptr);
			static get_partram_t get_partram;

			static char* KeybladePAX;

			static void ChangeWeapon(char* task, int part, bool hand_secondary, int item, bool playPax = true);
		};
	}
}