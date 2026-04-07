#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT MESSAGEDRAW
        {
        public:
            using CreateNew_t = void(*)(char* MessageDraw, int type, char* message, int size, int align);
            using set_line_space_t = void(*)(char* MessageDraw, int space);
            using draw_t = void(*)(char* MessageDraw, int x, int y, uint32_t color);

            static CreateNew_t CreateNew;
            static set_line_space_t set_line_space;
            static draw_t draw;
        };
    }
}