#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>

#include "memorymgr.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT PREF
        {
            public:
                static char** System;
        };
    }
}