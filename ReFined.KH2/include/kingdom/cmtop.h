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
        private:
            public:
                static inline char** (*GetListBuffer)() = FetchFunctionFromCall<char** (*)()>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x000E);
                static inline char* (*GetTopPlateSeqTbl)(int num) = FetchFunctionFromCall<char* (*)(int)>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x007C);
                static inline int (*GetFontColorSeqNum)(int num) = FetchRelativePointer<int(*)(int)>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x0076);
                static inline int (*GetItemFontColor)(int id, int cnt, bool special) = FetchRelativePointer<int(*)(int, int, bool)>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x01E6);
                static inline int (*GetCurPos)(int num) = FetchFunctionFromCall<int(*)(int)>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x01D5);
                static inline int (*GetSelectPos)(int num) = FindSignature<int(*)(int)>("\x48\x63\xC1\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0C\x00\x0F\xBF\x44\x11\x4C\x0F\xBF\x0C\x11\x03\xC1\xC3", "xxxxxx????xxxxxxxxxxxxxxxx");
                static inline char* (*GetScrBarSeqTbl)() = FetchFunctionFromCall<char* (*)()>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", 0x475);

                static inline char** m_Lay = FetchRelativePointer<char**>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x00B0);
                static inline char** m_MenuPtr = FetchRelativePointer<char**>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x009F);
                static inline char** m_SclBar = FetchRelativePointer<char**>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", 0x484);
                static inline char** m_ItemInfo = FetchRelativePointer<char**>("\x40\x53\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x28\x48\x89\x7C\x24\x60\x4C\x89\x74\x24\x20\xE8", "xxxxxxxxxxxxxxxxxxxxxxx", 0x0104);

                static inline int* m_SeqUnit = FetchRelativePointer<int*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0098);
                static inline int* m_ImgUnit = FetchRelativePointer<int*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x008E);
                static inline int* m_DummySeq = FetchRelativePointer<int*>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x0094);
        };
    }
}