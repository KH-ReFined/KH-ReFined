#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT MESSAGEDRAW
        {
        public:
            static inline void(*_MESSAGEDRAW)(char* MessageDraw, int type, char* message, int size, int align) = FetchFunctionFromCall<void(*)(char*, int, char*, int, int)>("\x40\x56\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00\x00\x48\x8B\xF1\x48\x8B\x89", "xxxxxxxxxxxx????xxxxxxxxxxxxxxxxx", 0x1F8);
            static inline void(*set_line_space)(char* MessageDraw, int space) = FetchFunctionFromCall<void(*)(char*, int)>("\x40\x56\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00\x00\x48\x8B\xF1\x48\x8B\x89", "xxxxxxxxxxxx????xxxxxxxxxxxxxxxxx", 0x20D);
            static inline void(*draw)(char* MessageDraw, int x, int y, uint32_t color) = FetchFunctionFromCall<void(*)(char*, int, int, uint32_t)>("\x40\x56\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00\x00\x48\x8B\xF1\x48\x8B\x89", "xxxxxxxxxxxx????xxxxxxxxxxxxxxxxx", 0x225);
        };
    }
}