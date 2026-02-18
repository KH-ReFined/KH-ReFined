#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "region.h"
#include "file.h"
#include "area.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT EVENT
		{
		public:
			static uint64_t pint_eventinfo;
		};
	}

	namespace sa
	{
		class DLL_EXPORT EVENT
		{
		public:
			using get_read_wk_motion_t = char* (*)();
			static get_read_wk_motion_t get_read_wk_motion;

			using get_read_wk_audio_t = char* (*)(char* name, int type);
			static get_read_wk_audio_t get_read_wk_audio;

			using getSkeletonName_t = char* (*)(int entryId);
			static getSkeletonName_t getSkeletonName;

			using readBuffAlloc_t = char* (*)(char* name, char* wk, int allocType);
			static readBuffAlloc_t readBuffAlloc;

			static YS::FILE::ReadCallback ReadMotionCallback;
			static YS::FILE::ReadCallback ReadAudioCallback;

			static char* MotionFilename;
			static char* AudioFilename;

			static char* motion_read_set(char* name, int allocType, uint32_t entryId, int put_id, uint8_t deleteFlag);
			static char* audio_read_set(char* name, int allocType);
		};
	}
}