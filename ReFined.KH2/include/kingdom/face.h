#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "area.h"
#include "obj2d.h"
#include "binarc.h"
#include "sprite.h"
#include "image.h"
#include "menu.h"
#include "image_cache.h"
#include "converter.h"
#include "memorymgr.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT FACE
        {
        public:
            static inline bool (*getFaceSed)(char* face, char** sqd, char* object) = FindSignature<bool(*)(char*, char**, char*)>("\x40\x55\x48\x83\xEC\x20\xC6\x81\x04\x03\x00\x00\x00\x49\x8B\xC0", "xxxxxxxxxxxxxxxx");

            static void create(char* face, int priority, int type, char* object, int status, int group, int offset16x9)
            {
                char* _fetchFaceSqd = nullptr;
                *reinterpret_cast<uint32_t*>(face + 0x300) = type;

                if (dk::FACE::getFaceSed(face, &_fetchFaceSqd, object) == 1)
                {
                    auto _statusCheck = 0x00;

                    if (!status || status != 0x01 && (_statusCheck = 0x02, status != 0x02))
                        _statusCheck = 0x03;

                    dk::Sprite::create(face, priority, _fetchFaceSqd, face + 0x200, _statusCheck, group, offset16x9);

                    auto _allocCrown = (char*)malloc(0x328);
                    auto _fetchEmpty = &dk::FACE::emptyDraw;

                    memset(_allocCrown, 0x00, 0x328);
                    dk::Sprite::_Sprite(_allocCrown);

                    memcpy(_allocCrown + 0x300, *reinterpret_cast<char**>(_allocCrown), 0x28);

                    memcpy(_allocCrown + 0x308, &_fetchEmpty, 0x08);
                    memcpy(_allocCrown + 0x310, &_fetchEmpty, 0x08);

                    *reinterpret_cast<char**>(_allocCrown) = _allocCrown + 0x300;
                    *reinterpret_cast<char**>(face + 0x0308) = _allocCrown;

                    auto _fetchObjectBinarc = *reinterpret_cast<char**>(object + 0x0928);

                    auto _intptrCrownIMD = YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x18, 0x6E777263, 0);
                    auto _intptrCrownSQD = YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x19, 0x6E777263, 0);

                    if (_intptrCrownIMD && _intptrCrownSQD)
                    {
                        auto _addressCrownIMD = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_intptrCrownIMD + 0x08));
                        auto _addressCrownSQD = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_intptrCrownSQD + 0x08));

                        YI::IMAGE::_IMAGE(_allocCrown + 0x200);

                        YI::IMAGE::Init(_allocCrown + 0x200, _addressCrownIMD);

                        *reinterpret_cast<uint32_t*>(_allocCrown + 0x210) = *reinterpret_cast<uint32_t*>(face + 0x210);
                        *reinterpret_cast<uint32_t*>(_allocCrown + 0x22C) = *reinterpret_cast<uint32_t*>(face + 0x22C);

                        YI::IMAGE::MakePacket(_allocCrown + 0x200);
                        YI::IMAGE::InitLoadImage(_allocCrown + 0x200);

                        dk::Sprite::create(_allocCrown, -1, _addressCrownSQD, _allocCrown + 0x200, status, group, offset16x9);

                        *reinterpret_cast<int*>(face + 0x0310) = 1;
                    }

                    else
                    {
                        dk::Sprite::create(_allocCrown, -1, nullptr, nullptr, status, group, offset16x9);
                        *reinterpret_cast<int*>(face + 0x0310) = 0;
                    }
                }

                else
                {
                    dk::Obj2D::create(face, priority, group);

                    *reinterpret_cast<uint32_t*>(face + 0x1DC) = offset16x9;
                    *reinterpret_cast<uint32_t*>(face + 0x1D8) = UINT32_MAX;

                    dk::Sprite::initWork(face);
                }
            }

            static void reload(char* face, char* object, int status)
            {
                char* _fetchFaceSqd = nullptr;

                if (dk::FACE::getFaceSed(face, &_fetchFaceSqd, object) == 1)
                {
                    auto _statusCheck = 0x00;

                    if (!status || status != 0x01 && (_statusCheck = 0x02, status != 0x02))
                        _statusCheck = 0x03;

                    YI::SEQUENCE::Init(face + 0x20, _fetchFaceSqd, face + 0x0200);
                    YI::SEQUENCE::SetNumberForce(face + 0x20, _statusCheck);

                    *reinterpret_cast<uint32_t*>(face + 0x1F0) = 0x00;
                    *reinterpret_cast<uint32_t*>(face + 0x1D8) = UINT32_MAX;
                    *reinterpret_cast<uint32_t*>(face + 0x1D4) = _statusCheck;

                    auto _fetchObjectBinarc = *reinterpret_cast<char**>(object + 0x0928);

                    if (YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x18, 0x6E777263, 0) != 0x00)
                    {
                        auto _allocCrown = *reinterpret_cast<char**>(face + 0x0308);

                        if (_allocCrown)
                        {
                            auto _addressCrownIMD =PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x18, 0x6E777263, 0) + 0x08));
                            auto _addressCrownSQD =PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x19, 0x6E777263, 0) + 0x08));

                            YI::IMAGE::Init(_allocCrown + 0x200, _addressCrownIMD);

                            *reinterpret_cast<uint32_t*>(_allocCrown + 0x210) = *reinterpret_cast<uint32_t*>(face + 0x210);
                            *reinterpret_cast<uint32_t*>(_allocCrown + 0x22C) = *reinterpret_cast<uint32_t*>(face + 0x22C);

                            YI::IMAGE::MakePacket(_allocCrown + 0x200);
                            YI::IMAGE::InitLoadImage(_allocCrown + 0x200);

                            YI::SEQUENCE::Init(_allocCrown + 0x20, _addressCrownSQD, _allocCrown + 0x0200);
                            YI::SEQUENCE::SetNumberForce(_allocCrown + 0x20, _statusCheck);

                            *reinterpret_cast<uint32_t*>(_allocCrown + 0x1F0) = 0x00;
                            *reinterpret_cast<uint32_t*>(_allocCrown + 0x1D8) = UINT32_MAX;
                            *reinterpret_cast<uint32_t*>(_allocCrown + 0x1D4) = _statusCheck;

                            *reinterpret_cast<int*>(face + 0x0310) = 1;
                        }
                    }

                    else
                        *reinterpret_cast<int*>(face + 0x0310) = 0;
                }
            }

            static void draw(char* face)
            {
                auto _fetchCrown = *reinterpret_cast<char**>(face + 0x0308);
                auto _fetchCrownIMD = *reinterpret_cast<char**>(face + 0x0310);
                auto _fetchIsDraw = *(face + 0x304);
                auto _fetchIsDrawCrown = *(face + 0x0310);

                if (_fetchIsDraw)
                {
                    YI::IMAGE::LoadTexture(face + 0x200);
                    dk::Sprite::draw(face);

                    auto _crownItemsArray = new char[0x03];
                    memcpy(_crownItemsArray, AREA::SaveData + 0x36B2, 0x03);

                    uint8_t _calculateCrown = _crownItemsArray[0] + _crownItemsArray[1] + _crownItemsArray[2];

                    if (_calculateCrown > 3)
                        _calculateCrown = 3;

                    if (_fetchCrown && _calculateCrown > 0 && _fetchIsDrawCrown)
                    {
                        auto _fetchFaceNum = *reinterpret_cast<uint32_t*>(face + 0x1D4) + 0x04 * (_calculateCrown - 1);
                        auto _fetchFaceTime = *reinterpret_cast<float*>(face + 0x168);

                        if (*reinterpret_cast<uint32_t*>(_fetchCrown + 0x1D4) != _fetchFaceNum || _fetchFaceTime == 0x00)
                            dk::Sprite::setNum(_fetchCrown, _fetchFaceNum);

                        *reinterpret_cast<int*>(_fetchCrown + 0x1C0) = *reinterpret_cast<int*>(face + 0x1C0);
                        *reinterpret_cast<int*>(_fetchCrown + 0x1C4) = *reinterpret_cast<int*>(face + 0x1C4);

                        YI::IMAGE::LoadTexture(_fetchCrown + 0x200);

                        dk::Sprite::draw(_fetchCrown);
                    }
                }
            }
            
            static void update(char* face)
            {
                auto _fetchCrown = *reinterpret_cast<char**>(face + 0x0308);
                auto _fetchIsDraw = *(face + 0x304);
                auto _fetchIsDrawCrown = *(face + 0x0310);

                if (_fetchIsDraw)
                {
                    dk::Sprite::update(face);

                    if (_fetchCrown && _fetchIsDrawCrown)
                        dk::Sprite::update(_fetchCrown);
                }
            }

            static void emptyDraw(char* sprite) { };

        private:
            static bool _init()
            {
                RedirectFunction("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x8B\xF2\x44\x89\x81\x00\x03\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(create), 0xBF);
                RedirectFunction("\x80\xB9\x04\x03\x00\x00\x00\x0F\x85", "xxxxxxxxx", reinterpret_cast<uint64_t>(update), 0x0E);
                RedirectFunction("\x40\x53\x48\x83\xEC\x20\x80\xB9\x04\x03\x00\x00\x00\x48\x8B\xD9\x74\x19\x48\x81\xC1", "xxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(draw), 0x35);
                RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x41\x8B\xD8\x33\xFF\x4C\x8B\xC2\x48", "xxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(reload), 0x35);

                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif
        };
    }
}