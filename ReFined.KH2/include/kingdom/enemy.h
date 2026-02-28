#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>

#include "memorymgr.h"
#include "file.h"
#include "form_level.h"
#include "obj.h"
#include "area.h"
#include "converter.h"
#include "saveram_battle.h"
#include "region.h"
#include "cache_buff.h"

using namespace std;

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT ENEMY
        {
        public:
            static char* LastAttacker;
            static char* _OVR__dead(char* enemy);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning YS::ENEMY...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _OVR__dead_orig = SignatureScan<char*>("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x89\x38\x0D\x00\x00\xE8", "xxxxxxxxxxxxxxxx");
                    uint64_t _constFunction = reinterpret_cast<uint64_t>(YS::ENEMY::_OVR__dead);

                    printf("Fetched YS::ENEMY::_OVR__dead @ 0x%p\n", _OVR__dead_orig);

                    fill(_OVR__dead_orig, _OVR__dead_orig + 0x98, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
                    memcpy(_OVR__dead_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked YS::ENEMY::_OVR__dead [0x%p] to Re:Fined function @ 0x%p\n", _OVR__dead_orig, YS::ENEMY::_OVR__dead);

                    printf("\nSuccessfully handled YS::ENEMY concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}