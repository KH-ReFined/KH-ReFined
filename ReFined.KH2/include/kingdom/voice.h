#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "SigScan.h"

#include "area.h"
#include "areainfo.h"
#include "objentry.h"
#include "region.h"
#include "file.h"
#include "world.h"
#include "task.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT VOICE
		{
		public:
			static char* Cache;
			static char** CurrentTask;

			static uint64_t* Part;

			static YS::FILE::ReadCallback ReadCallback;

			static void ReadEntryId(uint16_t entryID, char* buff);
		};
	}
}