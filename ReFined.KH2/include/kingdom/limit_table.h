#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "sound.h"
#include "region.h"
#include "memorymgr.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT LIMIT_TABLE
        {
            public:
                static char* LIMIT_FNBUFFER;

                static char* get_filename(char* limitTable)
                {
                    const char* _regionStr = (!YS::REGION::Get() || YS::REGION::Get() == 0x07) ? "fm" : reinterpret_cast<char*>(*YS::REGION::Region);

                    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

                    string _fetchPath = _fetchConfig & 0x0200 ? "limit_2nd" :
                        (_fetchConfig & 0x0400 ? "limit_3rd" : "limit");

                    char* _limitName = limitTable + 0x04;

                    sprintf_s(YS::LIMIT_TABLE::LIMIT_FNBUFFER, 0x28, "%s/%s/%s", _fetchPath.c_str(), _regionStr, _limitName);

                    if (!YS::FILE::GetSize(YS::LIMIT_TABLE::LIMIT_FNBUFFER))
                        sprintf_s(YS::LIMIT_TABLE::LIMIT_FNBUFFER, 0x28, "limit/%s/%s", _regionStr, _limitName);

                    return YS::LIMIT_TABLE::LIMIT_FNBUFFER;
                }

            private:
                static bool _init()
                {
                    auto _fetchFilenameOriginal = FetchFunctionFromCall<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x48\x8B\xF9\x0F\x29\x74\x24\x30\x8B\x09\x0F\x28\xF1\xE8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x24);

                    LIMIT_FNBUFFER = FetchRelativePointer<char*>(_fetchFilenameOriginal, 0x4D);
                    RedirectFunction(_fetchFilenameOriginal, reinterpret_cast<uint64_t>(get_filename), 0x69);
                    return true;
                }

                #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
                static inline bool _doInit = _init();
                #endif
        };
    }
}