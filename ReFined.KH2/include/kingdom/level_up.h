#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include "sprite.h"
#include "spritemessage.h"
#include "messagedraw.h"
#include "converter.h"
#include "area.h"
#include "memorymgr.h"
#include "message.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT LEVEL_UP
        {
        private:
            static bool _init()
            {
                auto _fetchVtable = FetchRelativePointer<char*>("\x40\x53\x57\x48\x83\xEC\x28\x48\x8B\x05", "xxxxxxxxxx", 0x56);

                auto _drawFunc = (uint64_t)draw;

                RedirectFunction(*reinterpret_cast<char**>(_fetchVtable + 0x10), reinterpret_cast<uint64_t>(draw), 0xBA);
                RedirectFunction(*reinterpret_cast<char**>(_fetchVtable + 0x40), reinterpret_cast<uint64_t>(draw), 0xBA);
                RedirectFunction(*reinterpret_cast<char**>(_fetchVtable + 0x70), reinterpret_cast<uint64_t>(draw), 0xBA);

                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

        public:
            static void draw(char* levelUp)
            {
                auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
                auto _offsetValue = 85;

                if (_fetchMemory)
                    memcpy(&_offsetValue, _fetchMemory, 0x04);

                char _messageMemory[0x300];

                if (*reinterpret_cast<int*>(levelUp + 0x021C) == 0x00)
                    dk::Sprite::draw(levelUp);

                dk::SpriteMessage::drawMessage(levelUp);

                auto _activeX = YI::SEQUENCE::GetActiveX(levelUp + 0x0020) + *reinterpret_cast<int*>(levelUp + 0x01C0);
                auto _activeY = YI::SEQUENCE::GetActiveY(levelUp + 0x0020) + *reinterpret_cast<int*>(levelUp + 0x01C4);

                auto _activeRGBA = *reinterpret_cast<uint32_t*>(levelUp + 0x0208) & 0xFFFFFF | YI::SEQUENCE::GetActiveRGBA(levelUp + 0x0020) & 0xFF000000;

                auto _msgSize = *reinterpret_cast<uint32_t*>(levelUp + 0x020C);

                auto _messageName = *reinterpret_cast<char**>(levelUp + 0x0220);
                auto _messageReward = *reinterpret_cast<char**>(levelUp + 0x0228);

                auto _paramEx = YI::SEQUENCE::GetParamEx(levelUp + 0x0020);

                YS::MESSAGEDRAW::_MESSAGEDRAW(_messageMemory, 0x01, _messageName, _msgSize, 0x01);
                YS::MESSAGEDRAW::draw(_messageMemory, _activeX + (_activeX > 255 ? _offsetValue : _offsetValue * -1), _activeY, _activeRGBA);

                if (*reinterpret_cast<uint32_t*>(levelUp + 0x0234) != 0x00 || _activeX > 255)
                {
                    char _messageBuffer[0x10];
                    auto _sprintMessage = YS::MESSAGE::Sprintf(_messageBuffer, "%2d", *reinterpret_cast<uint32_t*>(levelUp + 0x0230));

                    YS::MESSAGEDRAW::_MESSAGEDRAW(_messageMemory + 0x0100, 0x01, _sprintMessage, _msgSize, 0x00);
                    YS::MESSAGEDRAW::draw(_messageMemory + 0x0100, _activeX + _paramEx + (_activeX > 255 ? _offsetValue : _offsetValue * -1), _activeY, _activeRGBA);
                }

                if (_messageReward)
                {
                    auto _msgFont = *reinterpret_cast<uint32_t*>(levelUp + 0x0210);

                    auto _paramX = YI::SEQUENCE::GetParamX(levelUp + 0x0020);
                    auto _paramY = YI::SEQUENCE::GetParamY(levelUp + 0x0020);

                    YS::MESSAGEDRAW::_MESSAGEDRAW(_messageMemory + 0x0200, _msgFont, _messageReward, _msgSize, 0x00);
                    YS::MESSAGEDRAW::draw(_messageMemory + 0x0200, _activeX + _paramX + (_activeX > 255 ? _offsetValue : _offsetValue * -1), _activeY + _paramY + 17, _activeRGBA);
                }
            }
        };
    }
}