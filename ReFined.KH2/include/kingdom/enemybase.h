#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT ENEMYBASE
        {
            public:
                static inline char* (*_OVR__dead)(char* enemy) = FetchRelativePointer<char* (*)(char*)>("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x89\x38\x0D\x00\x00\xE8", "xxxxxxxxxxxxxxxx", 0x94);
        };
    }
}