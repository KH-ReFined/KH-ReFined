#include "limit_table.h"

char* YS::LIMIT_TABLE::LIMIT_FNBUFFER = nullptr; //Resolved at static initializer.

char* YS::LIMIT_TABLE::get_filename(char* limitTable)
{
    const char* _regionStr = (!YS::REGION::Get() || YS::REGION::Get() == 0x07) ? "fm" : reinterpret_cast<char*>(*YS::REGION::Region);

    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    string _fetchPath = _fetchConfig & 0x0200 ? "limit_2nd" :
                       (_fetchConfig & 0x0400 ? "limit_3rd" : "limit");

    char* _limitName = limitTable + 0x04;

    sprintf(YS::LIMIT_TABLE::LIMIT_FNBUFFER, "%s/%s/%s", _fetchPath.c_str(), _regionStr, _limitName);

    if (!YS::FILE::GetSize(YS::LIMIT_TABLE::LIMIT_FNBUFFER))
        sprintf(YS::LIMIT_TABLE::LIMIT_FNBUFFER, "limit/%s/%s", _regionStr, _limitName);

    return YS::LIMIT_TABLE::LIMIT_FNBUFFER;
}

YS::LIMIT_TABLE::staticInitializer initialize;