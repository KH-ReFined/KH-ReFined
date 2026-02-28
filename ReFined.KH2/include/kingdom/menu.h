#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

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

			static char** Config;
			static char** GameOver;
			static char** Camp2LD;
			static char** SubOptionSel;
			static char** DialogBase;

			static char** Jiminy;
		};
	}
}