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
        class DLL_EXPORT LayFile
        {
        public:
            static void alloc(char* layFile, char* name)
            {
                auto _cacheAlloc = YS::CACHE_BUFF::Alloc(name, -1);
                *reinterpret_cast<char**>(layFile + 0x08) = _cacheAlloc;

                *reinterpret_cast<uint32_t*>(layFile + 0x10) = YS::BINARC::get_file_num(*reinterpret_cast<char**>(layFile + 0x08), 0x1C);
                *reinterpret_cast<uint32_t*>(layFile + 0x18) = YS::BINARC::get_file_num(*reinterpret_cast<char**>(layFile + 0x08), 0x1D);
                *reinterpret_cast<uint32_t*>(layFile + 0x14) = YS::BINARC::get_file_num(*reinterpret_cast<char**>(layFile + 0x08), 0x19);
                *reinterpret_cast<uint32_t*>(layFile + 0x1C) = YS::BINARC::get_file_num(*reinterpret_cast<char**>(layFile + 0x08), 0x18);

                *(layFile + 0x20) = 0x01;
            }
        };
    };
}