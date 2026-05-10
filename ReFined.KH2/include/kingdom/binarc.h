#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <Windows.h>
#include "area.h"
#include "region.h"
#include "converter.h"
#include "file.h"
#include "axa.h"
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT BINARC
		{
		public:
            using modelCreateVB_t = void (*)(char* readAddr, int kind, char* param);
            static modelCreateVB_t modelCreateVB;

            using modelInitVB_t = void (*)(char* readAddr, bool doProcess);
            static modelInitVB_t modelInitVB;

			using get_info_by_tag_t = char* (*)(char* binarc, uint32_t type, uint32_t tag, int n);
			static get_info_by_tag_t get_info_by_tag;

            static void preSetup(char* binarc);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning YS::BINARC...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _fileNameFunc = (uint64_t)preSetup;
                    auto _fileName_orig = SignatureScan<char*>("\x41\x56\x48\x81\xEC\x00\x01\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxx");

                    printf("Fetched YS::BINARC::preSetup @ 0x%p\n", _fileName_orig);

                    memset(_fileName_orig, 0x90, 0x34A);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_fileNameFunc, 0x08);
                    memcpy(_fileName_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked YS::BINARC::preSetup [0x%p] to Re:Fined function @ 0x%p\n", _fileName_orig, preSetup);

                    printf("\nSuccessfully handled YS::BINARC concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}