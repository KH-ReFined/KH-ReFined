#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"
#include "messagedraw.h"
#include "sound.h"
#include "menu.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT INFORMATION_WINDOW
        {
        public:
            static void init(char* infoWindow, char* message);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::INFORMATION_WINDOW...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constDraw = (uint64_t)init;
                    auto _draw_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x57\x48\x81\xEC\xD0\x00\x00\x00\x48\x8B\x05", "xxxxxxxxxxxxxxxx");

                    printf("Fetched dk::INFORMATION_WINDOW::init @ 0x%p\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0xBA);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::INFORMATION_WINDOW::init [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, init);

                    printf("\nSuccessfully handled dk::INFORMATION_WINDOW concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}