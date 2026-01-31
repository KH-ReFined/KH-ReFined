#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT  AREA
		{
		public:
			struct DLL_EXPORT  SET
			{
				uint16_t Map;
				uint16_t Battle;
				uint16_t Event;
			};

			struct DLL_EXPORT  INFO
			{
				uint8_t World;
				uint8_t Room;
				uint8_t Entrance;
				uint8_t Padding;
				YS::AREA::SET Set;
			};

			using Change_t = void(*)(YS::AREA::INFO* self, YS::AREA::INFO area);
			static Change_t Change;

			using Exit_t = void(*)();
			static Exit_t Exit;

			using MapJump_t = void(*)(YS::AREA::INFO* area, uint32_t effectType, uint32_t delay, bool is_continue);
			static MapJump_t MapJump;

			using Alloc_t = char* (*)(size_t size);
			static Alloc_t Alloc;

			using Free_t = void(*)(char* address);
			static Free_t Free;

			static YS::AREA::INFO* Current;
			static bool* IsInMap;
			static char* SaveData;
			static char* BattleStatus;
			static char* IsVendor;

			static uint64_t pint_enemyinfo;
		};
	}
}