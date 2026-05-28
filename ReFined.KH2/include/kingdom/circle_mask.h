#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "menu.h"
#include "obj2d.h"
#include "sequence.h"
#include "memorymgr.h"
#include "panacea_alloc.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT CIRCLE_MASK
        {
        private:
            static bool _init()
            {
                RedirectFunction("\x40\x57\x48\x83\xEC\x50\x48\x8B\xF9\x48\x8B\x89\x40\x02", "xxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(draw), 0xF5);
                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

        public:
            static void draw(char* MASK)
            {
                auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
                auto _isHudDraw = true;

                if (_fetchHudDraw)
                    memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

                if (!_isHudDraw && !*YS::MENU::IsMenu)
                    return;

                auto _fetchSprite = *reinterpret_cast<char**>(MASK + 0x0240);

                auto _instanceMain = *reinterpret_cast<char**>(MASK + 0x0020);
                auto _instanceSub = *reinterpret_cast<char**>(MASK + 0x0140);

                if (_fetchSprite)
                {
                    auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
                    auto _offsetValue = 85;

                    if (_fetchMemory)
                        memcpy(&_offsetValue, _fetchMemory, 0x04);

                    auto _fetchAspect = *reinterpret_cast<int*>(_fetchSprite + 0x024C);
                    auto _applyAspect = _fetchAspect == 0x00 ? 0x00 : (_fetchAspect > 0x00 ? _offsetValue : _offsetValue * -1);

                    auto _activeX = YI::SEQUENCE::GetActiveX(_fetchSprite + 0x20) + *reinterpret_cast<int*>(_fetchSprite + 0x01C0) + _applyAspect;
                    auto _activeY = YI::SEQUENCE::GetActiveY(_fetchSprite + 0x20) + *reinterpret_cast<int*>(_fetchSprite + 0x01C4);

                    *reinterpret_cast<short*>(MASK + 0x002C) = _activeX;
                    *reinterpret_cast<short*>(MASK + 0x002E) = _activeY;

                    if (*reinterpret_cast<int*>(MASK + 0x0248) == 1)
                    {
                        *reinterpret_cast<short*>(MASK + 0x014C) += _activeX;
                        *reinterpret_cast<short*>(MASK + 0x014E) += _activeY;
                    }

                    reinterpret_cast<void(*)(char*)>(*reinterpret_cast<char**>(_instanceMain + 0x08))(MASK + 0x0020);

                    if (*reinterpret_cast<int*>(MASK + 0x0248) == 1)
                        reinterpret_cast<void(*)(char*)>(*reinterpret_cast<char**>(_instanceSub + 0x08))(MASK + 0x0140);
                }
            }
        };
    }
}