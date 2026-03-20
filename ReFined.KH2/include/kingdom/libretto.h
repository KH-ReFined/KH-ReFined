#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "region.h"
#include "cache_buff.h"
#include "area.h"
#include "file.h"

extern "C"
{
	namespace worldmap
	{
		class DLL_EXPORT Libretto
		{
		public:
			static void flush(char* libretto);

			struct staticInitializer
			{
				staticInitializer()
				{
					
					printf("======================================================\n");
					printf("Handling hooks and redirections concerning worldmap::Libretto...\n\n");

					vector<uint8_t> _injectInstructionSet =
					{
						0x48, 0x8B, 0xCE, // mov rcx, rsi
						0xE8, 0x00, 0x00, 0x00, 0x00, // call [function]
						0xE9, 0x00, 0x00, 0x00, 0x00  // jmp [offset]
					};

					auto _parseLibrettoFunc = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x55\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x60\x48\x8B\x41", "xxxxxxxxxxxxxxxxxxxxxxxx");

					printf("Fetched YS::LIBRETTO::??? @ 0x%p\n", _parseLibrettoFunc);

					auto _flushFunc_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\xE8\x00\x00\x00\x00\x85\xC0\x74\x0E\xE8\x00\x00\x00\x00\x83\xF8\x07\x74\x04\x32\xC0\xEB\x02\xB0\x01\x44\x8B\x0F\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxx????xxxxx????xxxxxxxxxxxxxxxxx????");

					printf("Fetched worldmap::Libretto::flush @ 0x%p\n", _flushFunc_orig);

					uint32_t _calcCallOffset = _flushFunc_orig - (_parseLibrettoFunc + 0x136);
					uint32_t _calcJumpOffset = (_parseLibrettoFunc + 0x62C) - (_parseLibrettoFunc + 0x13B);

					memset(_parseLibrettoFunc + 0x12E, 0x90, 0x5D);

					memcpy(_injectInstructionSet.data() + 0x04, &_calcCallOffset, 0x04);
					memcpy(_injectInstructionSet.data() + 0x09, &_calcJumpOffset, 0x04);
					memcpy(_parseLibrettoFunc + 0x12E, _injectInstructionSet.data(), _injectInstructionSet.size());

					vector<uint8_t> _absoluteInstructionJMP =
					{
						0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
					};

					printf("Rewrote YS::LIBRETTO::??? [0x%p] to use worldmap::Libretto::flush!\n", _parseLibrettoFunc);

					auto _flushFunc = (uint64_t)flush;
					memset(_flushFunc_orig, 0x90, 0x0116);

					memcpy(_absoluteInstructionJMP.data() + 0x06, &_flushFunc, 0x08);
					memcpy(_flushFunc_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

					printf("Hooked worldmap::Libretto::flush [0x%p] to Re:Fined function @ 0x%p\n", _flushFunc_orig, flush);

					printf("\nSuccessfully handled worldmap::Libretto concerns.\n");
					printf("======================================================\n\n");
				}
			};

			static staticInitializer initialize;
		};
	}
}