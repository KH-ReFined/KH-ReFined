#include "anti_form.h"
#include <random>

int YS::ANTI_FORM::CheckForm(char* player, int form)
{
    auto _antiFormRam = AREA::SaveData + 0x24F0 + 0x0E04 + 0x05 * 0x38;
    auto _finalFormRam = AREA::SaveData + 0x24F0 + 0x0E04 + 0x04 * 0x38;

    if (form == 0x05)
    {
        auto _calcAntiPoints = *reinterpret_cast<int*>(_antiFormRam + 0x04) - *reinterpret_cast<int*>(*YS::PREF::System + 0x0A4);

        if (_calcAntiPoints <= 0)
            _calcAntiPoints = 0;

        *reinterpret_cast<int*>(_antiFormRam + 0x04) = _calcAntiPoints;

        return 5;
    }

    auto _calcAntiChance = *reinterpret_cast<int*>(_antiFormRam + 0x04) * YS::MISSION::GetAntiRate();

    if (YS::MISSION::GetAntiRate() != 0x00 && *reinterpret_cast<uint16_t*>(*reinterpret_cast<char**>(player + 0x5C0) + 0x1E4) & 0x8000)
        _calcAntiChance = 100;

    *reinterpret_cast<int*>(_antiFormRam + 0x04) += 0x01;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> randDist(1, 100);

    if (randDist(rng) >= _calcAntiChance || *AREA::BattleStatus == 0x00)
        return form;

    auto _doesHaveFinal = YS::ITEM::GetNumBackyard(0x1D) || YS::SACRIFICE::GetFormStatus(0x05) || *(*reinterpret_cast<char**>(player + 0x5C0) + 0x1B1) < *(YS::COMMAND_ELEM::GetDrive(0x05) + 0x1C);
    auto _ableToGetFinal = YS::PROGRESS::CheckFlag(0x4842);

    auto _calcFinalChance = fminf(*reinterpret_cast<float*>(*YS::PREF::System + 0x00A0), powf(*reinterpret_cast<float*>(*YS::PREF::System + 0x009C), static_cast<float>(*reinterpret_cast<int*>(_finalFormRam + 0x04))) * *reinterpret_cast<float*>(*YS::PREF::System + 0x0098));
    *reinterpret_cast<int*>(_finalFormRam + 0x04) += 0x01;

    if (!_doesHaveFinal && _ableToGetFinal && randDist(rng) <= _calcFinalChance)
    {
        *reinterpret_cast<int*>(_finalFormRam + 0x04) = 0x00;
        YS::ITEM::GetBackyard(0x1D, 0x01);

        auto _calcAntiPoints = *reinterpret_cast<int*>(_antiFormRam + 0x04) - *reinterpret_cast<int*>(*YS::PREF::System + 0x0A4);

        if (_calcAntiPoints <= 0)
            _calcAntiPoints = 0;

        *reinterpret_cast<int*>(_antiFormRam + 0x04) = _calcAntiPoints;

        return 5;
    }

    else
    {
        auto _calcAntiPoints = *reinterpret_cast<int*>(_antiFormRam + 0x04) - *reinterpret_cast<int*>(*YS::PREF::System + 0x090);

        if (_calcAntiPoints <= 0)
            _calcAntiPoints = 0;

        *reinterpret_cast<int*>(_antiFormRam + 0x04) = _calcAntiPoints;

        return 6;
    }
}

YS::ANTI_FORM::staticInitializer YS::ANTI_FORM::initialize;
