#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "itempic.h"
extern "C"
{
	namespace dk {

		class DLL_EXPORT TREASURE_INFO {
		public:
			using openPrizeWindow_t = void(*)(const char* message);
			static openPrizeWindow_t openPrizeWindow;

			static void openBoxWindow(const char* message, uint16_t image);
		};
	}
}