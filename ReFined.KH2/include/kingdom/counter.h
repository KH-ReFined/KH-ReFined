#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"
#include "sound.h"
#include "info_base.h"
#include "obj2d.h"
#include "sprite.h"
#include "sequence.h"
#include "information.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT COUNTER
		{
		public:
			using forceLeave_t = void(*)(char* counter);
			static forceLeave_t forceLeave;

			static void update(char* counter);
		};
	}
}