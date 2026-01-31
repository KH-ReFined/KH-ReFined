#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "SigScan.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SHEET
		{
		public:
			using Save_t = void(*)(char* playerSheet);
			static Save_t Save;

			using Load_t = void(*)(char* playerSheet, uint8_t* partRam);
			static Load_t Load;
		};
	}
}