#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <iomanip>
#include <iostream>

#include "SigScan.h"
#include "file.h"
#include "area.h"
#include "region.h"
#include "cache_buff.h"

using namespace std;

extern "C"
{
    namespace Tz
    {
        class DLL_EXPORT CmData
        {
        public:
            static char* MENU_FNAME_BUFFER;

            static char* MakeFname(char* buff, char* fileName);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("Handling hooks and redirections concerning Tz::CmData...\n\n");

                    MENU_FNAME_BUFFER = ResolveRelativeAddress<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0D);

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto MakeFName_orig = SignatureScan<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx");
                    uint64_t _constFunction = reinterpret_cast<uint64_t>(Tz::CmData::MakeFname);

                    printf("Fetched Tz::CmData::MakeFname @ 0x%p\n", MakeFName_orig);

                    fill(MakeFName_orig, MakeFName_orig + 0x191, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
                    memcpy(MakeFName_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmData::MakeFname [0x%p] to Re:Fined function @ 0x%p\n", MakeFName_orig, Tz::CmData::MakeFname);

                    printf("\nSuccessfully handled Tz::CmData concerns.\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}