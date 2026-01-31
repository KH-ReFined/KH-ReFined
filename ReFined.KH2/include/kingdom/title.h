#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
extern "C"
{
	extern "C"
	{
		namespace YS
		{
			class DLL_EXPORT TITLE
			{
			public:
				static bool* IsTitle;
				static char* Title2LD;
				static uint8_t* IntroSelect;
			};
		}
	}
}