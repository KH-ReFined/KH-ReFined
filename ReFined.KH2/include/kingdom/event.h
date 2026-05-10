#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "region.h"
#include "file.h"
#include "area.h"
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT EVENT
		{
		public:
			static char** Event;
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

			struct staticInitializer
			{
				staticInitializer()
				{
					#if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
						return;
					#endif

					printf("Handling hooks and redirections concerning sa::EVENT...\n\n");

					vector<uint8_t> _absoluteInstructionJMP =
					{
						0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
					};

					auto _anbLoadFunc = (uint64_t)motion_read_set;
					auto _anbLoad_orig = SignatureScan<char*>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x70\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x60\x41", "xxxxxxxxxxxxxx????xxxxxxxxx");

					printf("Fetched sa::EVENT::motion_read_set @ 0x%p\n", _anbLoad_orig);

					fill(_anbLoad_orig, _anbLoad_orig + 0x28F, 0x90);

					memcpy(_absoluteInstructionJMP.data() + 0x06, &_anbLoadFunc, 0x08);
					memcpy(_anbLoad_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

					printf("Hooked sa::EVENT::motion_read_set [0x%p] to Re:Fined function @ 0x%p\n", _anbLoad_orig, motion_read_set);


					auto _eventVoiceLoadFunc = (uint64_t)audio_read_set;
					auto _eventVoiceLoad_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xEA\x48\x8B\xF9\xBA\x03\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

					printf("Fetched sa::EVENT::audio_read_set @ 0x%p\n", _eventVoiceLoad_orig);

					fill(_eventVoiceLoad_orig, _eventVoiceLoad_orig + 0x12B, 0x90);

					memcpy(_absoluteInstructionJMP.data() + 0x06, &_eventVoiceLoadFunc, 0x08);
					memcpy(_eventVoiceLoad_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

					printf("Hooked sa::EVENT::audio_read_set [0x%p] to Re:Fined function @ 0x%p\n", _eventVoiceLoad_orig, audio_read_set);

					printf("\nSuccessfully handled sa::EVENT concerns.\n\n");

				}
			};
		
			static staticInitializer initialize;
		};
	}
}