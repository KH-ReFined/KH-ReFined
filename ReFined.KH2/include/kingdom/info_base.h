#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT INFO_BASE
		{
		public:
			using create_t = void(*)(char* base, int priority, int height, int draw_priority, int group, int align);

			static create_t create;
		};
	}
}