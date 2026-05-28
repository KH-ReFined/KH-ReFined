#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT PREF
        {
            public:
                static inline char** System = FetchRelativePointer<char**>("\x40\x53\x48\x83\xEC\x20\x45\x33\xC0\x48\x89\x0D\x00\x00\x00\x00\x4C\x8B\xD1", "xxxxxxxxxxxx????xxx", 0x1E8);
        };
    }
}