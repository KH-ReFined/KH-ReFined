#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"
#include "sound.h"
#include "info_base.h"
#include "obj2d.h"
#include "sprite.h"
#include "sequence.h"
#include "information.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT TIMER
        {
        public:
            using getTime_t = uint32_t(*)();
            static getTime_t getTime;

            using isDown_t = bool(*)();
            static isDown_t isDown;

            static void update(char* timer);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::TIMER...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _updateFunc = (uint64_t)update;
                    auto _update_orig = SignatureScan<char*>("\x40\x55\x53\x56\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x60\x48\x8B\xD9\xE8", "xxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::TIMER::update @ 0x%p\n", _update_orig);

                    memset(_update_orig, 0x90, 0x44B);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_updateFunc, 0x08);
                    memcpy(_update_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::TIMER::update [0x%p] to Re:Fined function @ 0x%p\n", _update_orig, update);

                    printf("\nSuccessfully handled dk::TIMER concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}