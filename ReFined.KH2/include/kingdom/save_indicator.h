#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include <SigScan.h>

#include "obj2d.h"
#include "sprite.h"
#include "sequence.h"
#include "converter.h"
#include "binarc.h"
#include "image.h"
#include "exp.h"
#include "field2dd.h"
#include "file.h"
#include "info_base.h"
#include "lockon.h"
#include "area.h"
#include "image_cache.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT SAVE_INDICATOR
		{
		public:
			static char* VTABLE_CLASS;
			static char* VTABLE_SPRITE;

			static void init();
			static void create(int offset16x9);

		private:
			static char* instance;
			static char* layoutFile;
			static char* image;
		};
	}
}