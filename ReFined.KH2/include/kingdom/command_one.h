#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "spritemessage.h"
#include "sequence.h"
#include "messagedraw.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT COMMAND_ONE
        {
        public:
            static void draw(char* Command);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::COMMAND_ONE...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constDraw = (uint64_t)draw;
                    auto _draw_orig = SignatureScan<char*>("\x40\x57\x48\x83\xEC\x20\x33\xC0\x48\x8B\xF9\x89\x44\x24\x30\x89", "xxxxxxxxxxxxxxxx");

                    printf("Fetched dk::COMMAND_ONE::draw @ 0x%p\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0xA5);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::COMMAND_ONE::draw [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, draw);

                    printf("\nSuccessfully handled dk::COMMAND_ONE concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}