#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "SigScan.h"
#include "sound.h"

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

			static char* KeybladePAX;

			static void ChangeWeapon(char* task, int part, bool hand_secondary, int item, bool playPax = true);
		};
	}
}