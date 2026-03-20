#include "libretto.h"

void worldmap::Libretto::flush(char* libretto)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    string _constructPath = _fetchConfig & 0x0004 ? "libretto/jp/wm%d.bar" :
                           (_fetchConfig & 0x0008 ? "libretto/es/wm%d.bar" :
                           (_fetchConfig & 0x0010 ? "libretto/de/wm%d.bar" :
                           (_fetchConfig & 0x0020 ? "libretto/bg/wm%d.bar" : "libretto/us/wm%d.bar")));

    sprintf(libretto + 0x74, _constructPath.c_str(), *reinterpret_cast<uint32_t*>(libretto));

    if (!YS::FILE::GetSize(libretto + 0x74))
        sprintf(libretto + 0x74, "libretto/us/wm%d.bar", *reinterpret_cast<uint32_t*>(libretto));

    fprintf(stdout, "[worldmap::Libretto::flush] | Requested LIBRETTO read for: \"%s\"\n", libretto + 0x74);

    YS::CACHE_BUFF::ReadRequestSub(libretto + 0x74, -200, 0x00);
    YS::CACHE_BUFF::Flush(nullptr);
}

worldmap::Libretto::staticInitializer worldmap::Libretto::initialize;