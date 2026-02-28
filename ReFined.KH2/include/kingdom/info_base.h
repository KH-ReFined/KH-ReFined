#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT INFO_BASE
		{
		public:
			using create_t = void(*)(char* base, int priority, int height, int draw_priority, int group, int align);
			using update_t = void(*)(char* base);

			static create_t create;
			static update_t update;
			
			static char* sm_height;
		};
	}
}