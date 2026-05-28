#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"
#include "messagedraw.h"
#include "sound.h"
#include "menu.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT INFORMATION_WINDOW
        {
        private:
            static bool _init()
            {
                RedirectFunction("\x48\x89\x5C\x24\x18\x57\x48\x81\xEC\xD0\x00\x00\x00\x48\x8B\x05", "xxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(init), 0xBA);
                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

        public:
            static void init(char* infoWindow, char* message)
            {
                auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
                auto _isHudDraw = true;

                if (_fetchHudDraw)
                    memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

                if (!_isHudDraw && !*YS::MENU::IsMenu)
                    return;

                auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
                auto _offsetValue = 85;

                if (_fetchMemory)
                    memcpy(&_offsetValue, _fetchMemory, 0x04);

                auto _applyAspect = (_offsetValue * -1) + 85;

                char _messageDrawInst[0x100];

                SOUND::PlaySFX(0x22);
                *reinterpret_cast<int*>(infoWindow + 0x049C) &= ~2;
                *reinterpret_cast<int*>(infoWindow + 0x02B0) &= ~0x0040;
                YS::MESSAGEDRAW::_MESSAGEDRAW(_messageDrawInst, 0x00, message, 0, 0x00);

                int _paramX = YI::SEQUENCE::GetParamX(infoWindow + 0x0060);
                auto _messageEndPoint = *reinterpret_cast<int*>(_messageDrawInst + 0x70);

                *reinterpret_cast<float*>(infoWindow + 0x0240) = static_cast<float>(_paramX);

                *reinterpret_cast<int*>(infoWindow + 0x0498) = _paramX - _messageEndPoint;
                *reinterpret_cast<float*>(infoWindow + 0x04A0) = 180.0;
            }
        }
    }
}