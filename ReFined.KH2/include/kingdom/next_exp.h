#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "image.h"
#include "sequence.h"
#include "mission_gauge.h"
#include "mission_count.h"

extern "C"
{
    namespace dk 
    {
        class DLL_EXPORT NEXT_EXP
        {
            public:
                static void draw(char* nextExp)
                {
                    auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
                    auto _offsetValue = 85;

                    if (_fetchMemory)
                        memcpy(&_offsetValue, _fetchMemory, 0x04);

                    auto _applyAspect = (_offsetValue * -1) + 85;

                    auto _activeX = YI::SEQUENCE::GetActiveX(nextExp + 0x0058) + *reinterpret_cast<int*>(nextExp + 0x0200);
                    auto _activeY = YI::SEQUENCE::GetActiveY(nextExp + 0x0058) + *reinterpret_cast<int*>(nextExp + 0x0204);

                    auto _positionX = *reinterpret_cast<int*>(nextExp + 0x01F8) + _activeX;
                    auto _positionY = *reinterpret_cast<int*>(nextExp + 0x01FC) + _activeY;

                    auto _articleCount = *reinterpret_cast<int*>(nextExp + 0x0DB0);

                    for (int i = 0; i < _articleCount; i++)
                    {
                        auto _fetchSqd = nextExp + 0x03C0 + 0x01A0 * i;
                        auto _fetchOffsetX = nextExp + 0x0D90 + 0x0004 * i;

                        *reinterpret_cast<int*>(_fetchSqd - 0x04) = *reinterpret_cast<int*>(_fetchOffsetX) + _positionX + _applyAspect;
                        *reinterpret_cast<int*>(_fetchSqd) = _positionY;

                        YI::SEQUENCE::Draw(_fetchSqd - 0x0190);
                    }
                }

            private:
                static bool _init()
                {
                    auto _fetchDrawOriginal = FindAllSignature<char*>("\x40\x53\x55\x57\x48\x83\xEC\x30\x48\x8B\xE9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????");

                    RedirectFunction(_fetchDrawOriginal[0], reinterpret_cast<uint64_t>(draw), 0xBF);
                    RedirectFunction(_fetchDrawOriginal[1], reinterpret_cast<uint64_t>(draw), 0xBF);

                    return true;
                }

                #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
                static inline bool _doInit = _init();
                #endif
        };
    }
}