#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <iomanip>
#include <iostream>

#include "memorymgr.h"
#include "file.h"
#include "area.h"
#include "region.h"
#include "cache_buff.h"
#include "itempic.h"

using namespace std;

extern "C"
{
    namespace Tz
    {
        class DLL_EXPORT CmTop
        {
            public:
                static char** (*GetListBuffer)();
                static char* (*GetTopPlateSeqTbl)(int num);
                static int (*GetFontColorSeqNum)(int num);
                static int (*GetItemFontColor)(int id, int cnt, bool special);
                static int (*GetCurPos)(int num);
                static int (*GetSelectPos)(int num);
                static char* (*GetScrBarSeqTbl)();

                static char** m_Lay;
                static char** m_MenuPtr;
                static char** m_SclBar;
                static char** m_ItemInfo;

                static int* m_SeqUnit;
                static int* m_ImgUnit;
                static int* m_DummySeq;
        };
    }
}