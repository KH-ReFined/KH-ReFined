#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COMMAND_DRAW
		{
		public:
			static uint64_t pint_commandmenu;
			static uint64_t pint_commanddraw;
		};
	}
}