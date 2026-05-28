#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <map>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "obj.h"
#include "area.h"
#include "file.h"
#include "region.h"
#include "enemybase.h"
#include "memorymgr.h"
#include "converter.h"
#include "cache_buff.h"
#include "form_level.h"
#include "saveram_battle.h"

using namespace std;

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT ENEMY
        {
        private:
            static bool _init()
            {
                RedirectFunction("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x89\x38\x0D\x00\x00\xE8", "xxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(_OVR__dead), 0x98);

                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

        public:
            static inline char* LastAttacker = FetchRelativePointer<char*>("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x89\x38\x0D\x00\x00\xE8", "xxxxxxxxxxxxxxxx", 0x34);


            static char* _OVR__dead(char* enemy)
            {
                auto _enemyPartNum = YS::OBJ::get_part_num(enemy);

                auto _attackerObjPtr = *reinterpret_cast<uint32_t*>(enemy + 0xD38);
                auto _attackerObj = PC::CONVERTER::INTPTR_TO_POINTER(_attackerObjPtr);

                if (YS::OBJ::is_exist(_attackerObj))
                {
                    YS::ENEMY::LastAttacker = _attackerObj;
                    auto _attackerFlags = *reinterpret_cast<uint32_t*>(_attackerObj + 0x6C8);

                    if (_attackerFlags & 0x80)
                        YS::FORM_LEVEL::CheckWisdomAndFinal(_enemyPartNum, _attackerObj);
                }

                YS::SAVERAM_BATTLE::inc_kill_count(AREA::SaveData + 0x24F0, _enemyPartNum);
                return YS::ENEMYBASE::_OVR__dead(enemy);
            }
        };
    }
}