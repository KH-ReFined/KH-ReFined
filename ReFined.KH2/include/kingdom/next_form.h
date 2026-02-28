#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>

#include "obj2d.h"
#include "sprite.h"
#include "sequence.h"
#include "exp.h"
#include "field2dd.h"
#include "area.h"
#include "info_base.h"
#include "lockon.h"
#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT NEXT_FORM
		{
		public:
			static char* VTABLE_CLASS;
			static char* VTABLE_SPRITE;

			static char* instance;

			static void create(int nextExp, int offset16x9);
			static void initNumber(int offset16x9);
		};
	}
}