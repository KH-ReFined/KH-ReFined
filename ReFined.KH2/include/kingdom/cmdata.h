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
        class DLL_EXPORT CmData
        {
        private:
            static bool _init()
            {
                RedirectFunction("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(MakeFname), 0x191);
                
                auto _backReadImage = FindSignature<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x");

                vector<uint8_t> _patchBackReadImage =
                {
                    0x48, 0x8B, 0xCC, // mov rcx, rsp
                    0xFF, 0x15, 0x02, 0x00, 0x00, 0x00, 0xEB, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // call [someFunction] 
                };

                memset(_backReadImage + 0x7D, 0x90, 0x14);

                auto _constFunction = (uint64_t)MakeFnameItempic;

                memcpy(_patchBackReadImage.data() + 0x0B, &_constFunction, 0x08);
                memcpy(_backReadImage + 0x7D, _patchBackReadImage.data(), _patchBackReadImage.size());

                memset(_backReadImage + 0x231, 0x90, 0x14);

                _constFunction = (uint64_t)MakeFnameFace;

                _patchBackReadImage[2] = 0xCD;

                memcpy(_patchBackReadImage.data() + 0x0B, &_constFunction, 0x08);
                memcpy(_backReadImage + 0x231, _patchBackReadImage.data(), _patchBackReadImage.size());

                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

        public:
            static inline char* MENU_FNAME_BUFFER = FetchRelativePointer<char*>("\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF2\x48\x2B\xD0\x48\x8B\xF9\x66\x0F\x1F\x44\x00\x00\x44\x0F\xB6\x00\x0F\xB6\x0C\x10\x44\x2B\xC1", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0D);
            static inline char* FAC_WRITE_BUFFER = FetchRelativePointer<char*>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x", 0x1F9);

            static inline char* FileInfo = FetchRelativePointer<char*>("\x48\x63\xC1\x48\x8D\x0C\x40\x48\x8D\x05\x00\x00\x00\x00\x48\x83\x7C", "xxxxxxxxxx????xxx", 0x0A);

            static char* MakeFname(char* buff, char* fileName)
            {
                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
                auto _regionPointer = (!YS::REGION::Get() || YS::REGION::Get() == 7) ? reinterpret_cast<char*>(*YS::REGION::DefaultRegion) : reinterpret_cast<char*>(*YS::REGION::Region);

                if (!strcmp(*reinterpret_cast<char**>(MENU_FNAME_BUFFER), fileName))
                {
                    string _constructFile = _fetchConfig & 0x0200 ? "file_2nd/%s/%s" : (_fetchConfig & 0x0400 ? "file_3rd/%s/%s" : "file/%s/%s");

                    sprintf(buff, _constructFile.c_str(), _regionPointer, fileName);

                    if (!YS::FILE::GetSize(buff))
                    {
                        _constructFile.resize(_constructFile.size() - 0x03);
                        sprintf(buff, _constructFile.c_str(), fileName);

                        if (!YS::FILE::GetSize(buff))
                        {
                            sprintf(buff, "file/%s/%s", _regionPointer, fileName);

                            if (!YS::FILE::GetSize(buff))
                                sprintf(buff, "file/%s", fileName);
                        }
                    }

                    return buff;
                }

                else
                {
                    string _constructMenu = _fetchConfig & 0x0200 ? "menu_2nd/%s/%s" : (_fetchConfig & 0x0400 ? "menu_3rd/%s/%s" : "menu/%s/%s");

                    auto _checkPhoto = strstr(fileName, "jm_photo/");
                    sprintf(buff, _constructMenu.c_str(), _regionPointer, fileName);

                    if (!YS::FILE::GetSize(buff))
                    {
                        if (_checkPhoto)
                            goto SKIP_PHOTO;

                        _constructMenu.resize(_constructMenu.size() - 0x03);
                        sprintf(buff, _constructMenu.c_str(), fileName);

                        if (!YS::FILE::GetSize(buff))
                        {
                        SKIP_PHOTO:
                            sprintf(buff, "menu/%s/%s", _regionPointer, fileName);

                            if (!YS::FILE::GetSize(buff) && !_checkPhoto)
                                sprintf(buff, "menu/%s", fileName);
                        }
                    }

                    return buff;
                }
            }

            static void MakeFnameFace(uint16_t id)
            {
                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
                auto _writeBuff = *reinterpret_cast<char**>(Tz::CmData::FAC_WRITE_BUFFER);

                string _fetchFilePath(_writeBuff);

                _fetchFilePath = _fetchFilePath.substr(0x04, _fetchFilePath.size() - 0x04);
                _fetchFilePath = _fetchFilePath.substr(0x00, _fetchFilePath.size() - 0x04);

                string _constructPath = _fetchConfig & 0x0200 ? "menu_2nd" : (_fetchConfig & 0x0400 ? "menu_3rd" : "menu");

                _constructPath = _constructPath.append(_fetchFilePath);
                _constructPath = _constructPath.append("%d.imd");

                sprintf(_writeBuff, _constructPath.c_str(), id);

                if (!YS::FILE::GetSize(_writeBuff))
                {
                    _constructPath = "menu";

                    _constructPath = _constructPath.append(_fetchFilePath);
                    _constructPath = _constructPath.append("%d.imd");

                    sprintf(_writeBuff, _constructPath.c_str(), id);
                }

                printf("[Tz::CmData::MakeFnameFace] | Fulfilling FAC request for: \"%s\"\n", _writeBuff);
            }

            static void MakeFnameItempic(char* buff, uint16_t id)
            {
                buff = buff + 0x30;

                auto _fetchPicturePtr = *YS::ITEMPIC::ToLoadID;
                auto _fetchPictureID = 0x00;

                if (_fetchPicturePtr != nullptr)
                    _fetchPictureID = *_fetchPicturePtr;

                else
                    _fetchPictureID = id;

                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
                string _constructPath = _fetchConfig & 0x0200 ? "itempic_2nd/item-%03d.imd" : (_fetchConfig & 0x0400 ? "itempic_3rd/item-%03d.imd" : "itempic/item-%03d.imd");

                sprintf(buff, _constructPath.c_str(), _fetchPictureID);

                if (!YS::FILE::GetSize(buff))
                    sprintf(buff, "itempic/item-%03d.imd", _fetchPictureID);

                printf("[Tz::CmData::MakeFnameItempic] | Fulfilling ITEMPIC request for: \"%s\"\n", buff);
            }
        };
    }
}