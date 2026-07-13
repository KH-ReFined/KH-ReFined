#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "file.h"
#include "binarc.h"
#include "cache_buff.h"
#include "layfile.h"
#include "region.h"
#include "memorymgr.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace Tz
    {
        class DLL_EXPORT SubMenu
        {
            public:
                static char* SetLeadData(const char* fileName)
                {
                    char _filePath[0x80];
                    char* Tz__MenuItem__m_LayFile = moduleInfo.moduleStart + 0xBEE770;

                    sprintf_s(_filePath, 0x80, "file/%s/%s", *YS::REGION::Region, fileName);

                    if (!YS::FILE::GetSize(_filePath))
                        sprintf_s(_filePath, 0x80, "file/%s", fileName);

                    Tz::LayFile::alloc(Tz__MenuItem__m_LayFile, _filePath);
                    return *reinterpret_cast<char**>(Tz__MenuItem__m_LayFile + 0x08);
                }
        };
    };
}