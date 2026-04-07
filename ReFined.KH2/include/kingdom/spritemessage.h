#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"
#include "messagedraw.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT SpriteMessage
        {
        public:
            static void draw(char* Sprite);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::SpriteMessage...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constDraw = (uint64_t)draw;
                    auto _draw_orig = SignatureScan<char*>("\x40\x56\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00\x00\x48\x8B\xF1\x48\x8B\x89", "xxxxxxxxxxxx????xxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::SpriteMessage::draw @ 0x%p\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0x265);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::SpriteMessage::draw [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, draw);

                    printf("\nSuccessfully handled dk::SpriteMessage concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}