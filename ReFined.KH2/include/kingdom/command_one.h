#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "spritemessage.h"
#include "sequence.h"
#include "messagedraw.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT COMMAND_ONE
        {
        public:
            static void draw(char* Command)
            {
                auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
                auto _isHudDraw = true;

                if (_fetchHudDraw)
                    memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

                if (!_isHudDraw && !*MENU::IsMenu)
                    return;

                dk::Sprite::draw(Command);
                dk::SpriteMessage::drawMessage(Command);

                if (*reinterpret_cast<int*>(Command + 0x03C0) > 1)
                {
                    auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
                    auto _offsetValue = 85;

                    if (_fetchMemory)
                        memcpy(&_offsetValue, _fetchMemory, 0x04);

                    auto _applyAspect = (_offsetValue * -1);

                    *reinterpret_cast<int*>(Command + 0x018C + 0x0220) = YI::SEQUENCE::GetActiveX(Command + 0x0020) + *reinterpret_cast<int*>(Command + 0x01C0) + YI::SEQUENCE::GetParamX(Command + 0x0020) + _applyAspect;
                    *reinterpret_cast<int*>(Command + 0x0190 + 0x0220) = YI::SEQUENCE::GetActiveY(Command + 0x0020) + *reinterpret_cast<int*>(Command + 0x01C4) + YI::SEQUENCE::GetParamY(Command + 0x0020);

                    YI::SEQUENCE::Draw(Command + 0x0220);
                }
            }

        private:
            static bool _init()
            {
                RedirectFunction("\x40\x57\x48\x83\xEC\x20\x33\xC0\x48\x8B\xF9\x89\x44\x24\x30\x89", "xxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(draw), 0xA5);
                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif
        };
    }
}