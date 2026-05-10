#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT SECTOR_MASK
        {
        public:
            static void draw(char* Sprite);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::SECTOR_MASK...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constDraw = (uint64_t)draw;
                    auto _draw_orig = SignatureScan<char*>("\x40\x55\x48\x83\xEC\x50\x48\x8B\xE9\x48\x8B\x89\x40\x01", "xxxxxxxxxxxxxx");

                    printf("Fetched dk::SECTOR_MASK::draw @ 0x%p\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0xC5);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::SECTOR_MASK::draw [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, draw);

                    printf("\nSuccessfully handled dk::SECTOR_MASK concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}