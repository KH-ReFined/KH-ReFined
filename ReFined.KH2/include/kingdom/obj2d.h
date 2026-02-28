#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "info_base.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Obj2D
		{
		public:
			using isExist_t = bool(*)(char* obj);
			using commitSuicide_t = void(*)(char* obj, int unk1);
			using create_t = void(*)(char* obj, int priority, int group);

			static isExist_t isExist;
			static commitSuicide_t commitSuicide;
			static create_t create;

			static char** sm_manager_ptr;
		};
	}
}