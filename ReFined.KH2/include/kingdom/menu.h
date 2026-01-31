#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MENU
		{
		public:
			using PauseExecTask_t = void(*)(const char* self);
			static PauseExecTask_t PauseExecTask;

			using UpdateListShortcut_t = void(*)(uint32_t index);
			static UpdateListShortcut_t UpdateListShortcut;

			using CampStart_t = void(*)(int type, long param);
			static CampStart_t CampStart;

			static bool* IsMenu;
			static char* MenuType;
			static char* SubMenuType;

			static uint8_t* CampOptions;

			static uint64_t pint_config;
			static uint64_t pint_gameover;
			static uint64_t pint_camp2ld;
			static uint64_t pint_suboptionselect;
			static uint64_t pint_dialogbase;

			static uint64_t pint_jiminymenu;
		};
	}
}