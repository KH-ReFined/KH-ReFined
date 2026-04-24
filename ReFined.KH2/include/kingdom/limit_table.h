#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <Windows.h>
#include "region.h"
#include "memorymgr.h"
#include "sound.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT LIMIT_TABLE
        {
        public:
            static char* LIMIT_FNBUFFER;

            static char* get_filename(char* limitTable);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning YS::LIMIT_TABLE...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _fileNameFunc = (uint64_t)get_filename;
                    auto _fileName_orig = ResolveFunctionFromCall<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x48\x8B\xF9\x0F\x29\x74\x24\x30\x8B\x09\x0F\x28\xF1\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x24);

                    printf("Fetched YS::LIMIT_TABLE::get_filename @ 0x%p\n", _fileName_orig);

                    YS::LIMIT_TABLE::LIMIT_FNBUFFER = ResolveRelativeAddress<char*>(_fileName_orig, 0x4D);

                    printf("Resolved YS::LIMIT_TABLE::LIMIT_FNBUFFER [0x%p] from YS::LIMIT_TABLE::get_filename [0x%p]\n", YS::LIMIT_TABLE::LIMIT_FNBUFFER, _fileName_orig);

                    memset(_fileName_orig, 0x90, 0x69);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_fileNameFunc, 0x08);
                    memcpy(_fileName_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked YS::LIMIT_TABLE::get_filename [0x%p] to Re:Fined function @ 0x%p\n", _fileName_orig, get_filename);

                    printf("\nSuccessfully handled YS::LIMIT_TABLE concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}