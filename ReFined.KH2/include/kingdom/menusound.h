#pragma once

#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "file.h"
#include "binarc.h"
#include "cache_buff.h"
#include "region.h"
#include "memorymgr.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace Tz
    {
        class DLL_EXPORT MenuSound
        {
        public:
            static inline void (*PlayBgSE)(int soundId) = FetchFunctionFromCall<void (*)(int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xBA\x01\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xB9\xF6\x2E\x00\x00", "xxxxxxxxxxxxxxxx????x????xxxxxxxxx????x????xxxxx", 0x30);
            static inline void (*StopBgSE)(int soundId) = FindSignature<void (*)(int)>("\x40\x53\x48\x83\xEC\x20\x8B\xD9\x85\xC9\x75\x0A", "xxxxxxxxxxxx");
        };
    };
}