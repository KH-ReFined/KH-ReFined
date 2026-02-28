#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "memorymgr.h"

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

			struct staticInitializer
			{
				staticInitializer()
				{
					printf("======================================================\n");
					printf("Handling hooks and redirections concerning YS::VOICE...\n\n");

					vector<uint8_t> _absoluteInstructionJMP =
					{
						0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
					};

					auto _readEntryIdFunc = (uint64_t)ReadEntryId;
					auto _readEntryIdFunc_orig = SignatureScan<char*>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx");

					printf("Fetched YS::VOICE::ReadEntryId @ 0x%p\n", _readEntryIdFunc_orig);

					memset(_readEntryIdFunc_orig, 0x90, 0x9C);

					memcpy(_absoluteInstructionJMP.data() + 0x06, &_readEntryIdFunc, 0x08);
					memcpy(_readEntryIdFunc_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

					printf("Hooked YS::VOICE::ReadEntryId [0x%p] to Re:Fined function @ 0x%p\n", _readEntryIdFunc_orig, ReadEntryId);

					printf("\nSuccessfully handled YS::VOICE concerns.\n");
					printf("======================================================\n\n");
				}
			};

			static staticInitializer initialize;
		};
	}
}