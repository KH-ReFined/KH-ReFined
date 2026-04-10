#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include "sprite.h"
#include "spritemessage.h"
#include "messagedraw.h"
#include "converter.h"
#include "area.h"
#include "memorymgr.h"
#include "message.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT LEVEL_UP
        {
        public:
            static void draw(char* levelUp);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::LEVEL_UP...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _fetchVtable = ResolveRelativeAddress<char*>("\x40\x53\x57\x48\x83\xEC\x28\x48\x8B\x05", "xxxxxxxxxx", 0x56);

                    auto _drawFunc = (uint64_t)draw;
                    auto _drawFunc_orig = *reinterpret_cast<char**>(_fetchVtable + 0x10);
                    auto _drawFunc_form = *reinterpret_cast<char**>(_fetchVtable + 0x40);
                    auto _drawFunc_summ = *reinterpret_cast<char**>(_fetchVtable + 0x70);

                    printf("Fetched dk::LEVEL_UP::draw @ 0x%p\n", _drawFunc_orig);

                    memset(_drawFunc_orig, 0x90, 0xBA);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_drawFunc, 0x08);
                    memcpy(_drawFunc_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::LEVEL_UP::draw [0x%p] to Re:Fined function @ 0x%p\n", _drawFunc_orig, draw);

                    printf("Fetched dk::LEVEL_UP_FORM::draw @ 0x%p\n", _drawFunc_form);

                    memset(_drawFunc_form, 0x90, 0xBA);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_drawFunc, 0x08);
                    memcpy(_drawFunc_form, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::LEVEL_UP_FORM::draw [0x%p] to Re:Fined function @ 0x%p\n", _drawFunc_form, draw);

                    printf("Fetched dk::LEVEL_UP_SUMMON::draw @ 0x%p\n", _drawFunc_summ);

                    memset(_drawFunc_summ, 0x90, 0xBA);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_drawFunc, 0x08);
                    memcpy(_drawFunc_summ, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::LEVEL_UP_SUMMON::draw [0x%p] to Re:Fined function @ 0x%p\n", _drawFunc_summ, draw);

                    printf("\nSuccessfully handled dk::LEVEL_UP concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}