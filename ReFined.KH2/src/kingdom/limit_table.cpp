#include "limit_table.h"

char* YS::LIMIT_TABLE::LIMIT_FNBUFFER = nullptr; //Resolved at static initializer.

char* YS::LIMIT_TABLE::get_filename(char* limitTable)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    string _encodedRegion = _fetchConfig & 0x0004 ? "jp" :
        (_fetchConfig & 0x0008 ? "es" :
            (_fetchConfig & 0x0010 ? "de" :
                (_fetchConfig & 0x0020 ? "bg" : "us")));

    char* _limitName = limitTable + 0x04;

    const char* _regionStr = !YS::REGION::Get() || YS::REGION::Get() == 0x07 ? "fm" : reinterpret_cast<char*>(*YS::REGION::Region);

    sprintf(YS::LIMIT_TABLE::LIMIT_FNBUFFER, "limit/%s/%s", _encodedRegion.c_str(), _limitName);

    if (!YS::FILE::GetSize(YS::LIMIT_TABLE::LIMIT_FNBUFFER))
        sprintf(YS::LIMIT_TABLE::LIMIT_FNBUFFER, "limit/%s/%s", _regionStr, _limitName);

    return YS::LIMIT_TABLE::LIMIT_FNBUFFER;
}

YS::LIMIT_TABLE::staticInitializer initialize;