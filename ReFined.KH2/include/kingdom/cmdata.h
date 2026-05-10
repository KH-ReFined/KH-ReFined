#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <iomanip>
#include <iostream>

#include "memorymgr.h"
#include "file.h"
#include "area.h"
#include "region.h"
#include "cache_buff.h"
#include "itempic.h"

using namespace std;

extern "C"
{
    namespace Tz
    {
        class DLL_EXPORT CmData
        {
        public:
            static char call_statement[0x20];

            static char* MENU_FNAME_BUFFER;
            static char* FAC_WRITE_BUFFER;

            static char* FileInfo;

            static char* MakeFname(char* buff, char* fileName);
            static void MakeFnameItempic(char* buff, uint16_t id);
            static void MakeFnameFace(uint16_t id);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning Tz::CmData...\n\n");

                    MENU_FNAME_BUFFER = ResolveRelativeAddress<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0D);
                    FAC_WRITE_BUFFER = ResolveRelativeAddress<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x", 0x1F9);

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto MakeFName_orig = SignatureScan<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx");
                    auto _constFunction = (uint64_t)MakeFname;

                    printf("Fetched Tz::CmData::MakeFname @ 0x%p\n", MakeFName_orig);

                    fill(MakeFName_orig, MakeFName_orig + 0x191, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
                    memcpy(MakeFName_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmData::MakeFname [0x%p] to Re:Fined function @ 0x%p\n", MakeFName_orig, MakeFname);

                    auto _backReadImage = SignatureScan<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x");

                    printf("Fetched Tz::UnitLoad::BackReadImage @ 0x%p\n", _backReadImage);

                    vector<uint8_t> _patchBackReadImage =
                    {
                        0x48, 0x8B, 0xCC, // mov rcx, rsp
                        0xFF, 0x15, 0x02, 0x00, 0x00, 0x00, 0xEB, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // call [someFunction] 
                    };

                    memset(_backReadImage + 0x7D, 0x90, 0x14);

                    _constFunction = (uint64_t)MakeFnameItempic;

                    memcpy(_patchBackReadImage.data() + 0x0B, &_constFunction, 0x08);
                    memcpy(_backReadImage + 0x7D, _patchBackReadImage.data(), _patchBackReadImage.size());

                    printf("Redirected subroutine Tz::UnitLoad::BackReadImage [0x%p] + 0x07D to Re:Fined function @ 0x%p\n", _backReadImage, MakeFnameItempic);

                    memset(_backReadImage + 0x231, 0x90, 0x14);

                    _constFunction = (uint64_t)MakeFnameFace;

                    _patchBackReadImage[2] = 0xCD;

                    memcpy(_patchBackReadImage.data() + 0x0B, &_constFunction, 0x08);
                    memcpy(_backReadImage + 0x231, _patchBackReadImage.data(), _patchBackReadImage.size());

                    printf("Redirected subroutine Tz::UnitLoad::BackReadImage [0x%p] + 0x231 to Re:Fined function @ 0x%p\n", _backReadImage, MakeFnameFace);

                    printf("\nSuccessfully handled Tz::CmData concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}