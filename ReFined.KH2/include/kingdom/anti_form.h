#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <random>

#include "item.h"
#include "area.h"
#include "pref.h"
#include "mission.h"
#include "progress.h"
#include "sacrifice.h"
#include "command_elem.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ANTI_FORM
		{
            private:
                static bool _init()
                {
                    RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xF2\x48\x8B\xE9\xE8\x00\x00\x00\x00\x85\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx", reinterpret_cast<uint64_t>(CheckForm), 0x228);
                    return true;
                }

                #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
                static inline bool _doInit = _init();
                #endif

		    public:
			    static int CheckForm(char* player, int form)
                {
                    random_device _randomDevice;
                    mt19937 _randomAlgo(_randomDevice());
                    uniform_int_distribution<mt19937::result_type> _randomDist(1, 100);

                    auto _currAntiPoint = reinterpret_cast<int*>(AREA::SaveData + 0x24F0 + 0x0F20);
                    auto _currFinalAttempt = reinterpret_cast<int*>(AREA::SaveData + 0x24F0 + 0x0EE8);

                    auto _fetchLightDarkness = *reinterpret_cast<uint16_t*>(*reinterpret_cast<char**>(player + 0x5C0) + 0x1E4);

                    if (form == 0x05)
                    {
                        *_currAntiPoint -= *reinterpret_cast<int*>(*YS::PREF::System + 0x0A4);

                        if (*_currAntiPoint <= 0)
                            *_currAntiPoint = 0;

                        return 5;
                    }

                    auto _fetchAntiRate = *_currAntiPoint * YS::MISSION::GetAntiRate();
                    auto _calcAntiChance = (_fetchAntiRate <= 0x04 ? 0 : (_fetchAntiRate <= 0x09 ? 10 : 25));

                    if (YS::MISSION::GetAntiRate() != 0x00 && _fetchLightDarkness & 0x8000)
                        _calcAntiChance = 100;

                    *_currAntiPoint += 0x01;

                    if (_randomDist(_randomAlgo) >= _calcAntiChance || *AREA::BattleStatus == 0x00 || YS::SACRIFICE::GetFormStatus(0x06))
                        return form;

                    auto _playerHasFinal = YS::ITEM::GetNumBackyard(0x1D) || YS::SACRIFICE::GetFormStatus(0x05) || *(*reinterpret_cast<char**>(player + 0x5C0) + 0x1B1) < *(YS::COMMAND_ELEM::GetDriveForm(0x05) + 0x1C);
                    auto _playerCanFinal = YS::PROGRESS::CheckFlag(0x4842);

                    if (!_playerHasFinal && _playerCanFinal)
                    {
                        auto _calcFinalChance = fminf(*reinterpret_cast<float*>(*YS::PREF::System + 0x00A0), powf(*reinterpret_cast<float*>(*YS::PREF::System + 0x009C), static_cast<float>(*_currFinalAttempt)) * *reinterpret_cast<float*>(*YS::PREF::System + 0x0098));

                        if (_randomDist(_randomAlgo) <= _calcFinalChance)
                        {
                            *_currFinalAttempt = 0x00;
                            YS::ITEM::GetBackyard(0x1D, 0x01);

                            if (!YS::SACRIFICE::GetFormStatus(0x05))
                                return 5;

                            else
                                return form;
                        }

                        else
                            *_currFinalAttempt += 0x01;
                    }

                    *_currAntiPoint -= *reinterpret_cast<int*>(*YS::PREF::System + 0x090);

                    if (*_currAntiPoint <= 0)
                        *_currAntiPoint = 0;

                    return 6;
                }
		};
	}
}