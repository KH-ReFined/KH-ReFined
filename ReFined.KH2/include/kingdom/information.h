#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "image.h"
#include "sequence.h"
#include "mission_gauge.h"

extern "C"
{
	namespace dk {

		class DLL_EXPORT INFORMATION {
		public:
			using openInformationWindow_t = void(*)(const char* message);
			static openInformationWindow_t openInformationWindow;

			using openGetMunny_t = void(*)(int munny);
			static openGetMunny_t openGetMunny;
		};
	}
}