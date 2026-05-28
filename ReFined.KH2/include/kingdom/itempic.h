#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include "image.h"
#include "task.h"
#include "file.h"
#include "converter.h"
#include "area.h"
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ITEMPIC
		{
        private:
            static bool _init()
            {
                RedirectFunction("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", reinterpret_cast<uint64_t>(ReadImageThread), 0x115);
                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif

		public:
			static inline void(*ReadImage)(int _image) = FindSignature<void(*)(int)>("\x83\xF9\xFF\x74\x4D\x53\x48\x83\xEC\x40\x48\x8D\x05\x00\x00\x00\x00\xC7\x44\x24\x30\x8C\x00\x00\x00", "xxxxxxxxxxxxx????xxxxxxxx");
            static inline void(*FreeImageData)(char* _imageData) = FindSignature<void(*)(char*)>("\x48\x83\xEC\x28\x83\x2D\x00\x00\x00\x00\x01\xB9\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x0F\x44\xC1\x83\x3D\x00\x00\x00\x00\xFF\x89\x05\x00\x00\x00\x00\x74\x2E", "xxxxxx????xxxxxxxx????xxxxx????xxx????xx");

			static inline uint16_t* LoadedId = FetchRelativePointer<uint16_t*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\x0D\x00\x00\x00\x00\xB8\xFF\xFF\xFF\xFF\x33\xDB", "xxxxxxxxx????xxxxxxx", 0x2F);

			static inline char* ImageBuff = FetchRelativePointer<char*>("\x48\x83\xEC\x28\x83\x2D\x00\x00\x00\x00\x01\xB9\x00\x00\x00\x00\x8B\x05\x00\x00\x00\x00\x0F\x44\xC1\x83\x3D\x00\x00\x00\x00\xFF\x89\x05\x00\x00\x00\x00\x74\x2E", "xxxxxx????xxxxxxxx????xxxxx????xxx????xx", 0x2B);
			static inline uint16_t** ToLoadID = FetchRelativePointer<uint16_t**>("\x48\x83\xEC\x68\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x50\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x38\x05\x00\x00\x00\x00\x0F\x85\x00\x00\x00\x00\x66\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxx????xxxx????xx????xx????xxx????x", 0x4D);

			static inline uint32_t* Phase = FetchRelativePointer<uint32_t*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xE9);
			static inline uint32_t* UserNum = FetchRelativePointer<uint32_t*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xEF);
			static inline uint32_t* CachePic = FetchRelativePointer<uint32_t*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xE3);
			static inline char* Task = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0x105);

			static char* ReadImageThread(char* task)
            {
                char nameBuff[40];
                auto _itempicID = *reinterpret_cast<uint16_t*>(task + 0x18);

                fprintf(stdout, "[YS::ITEMPIC::ReadImageThread] | Requested file for ITEMPIC ID: %d\n", _itempicID);

                if (*Phase != 0x00)
                    TASK::sleep(task, 0);

                if (*CachePic != UINT32_MAX)
                {
                    auto _releaseResult = YI::IMAGE::ReleaseImage(ImageBuff);

                    if (_releaseResult)
                        *(_releaseResult + 0x99) = 0;

                    if (*reinterpret_cast<uint16_t*>(ImageBuff + 0x04) == 0x0100 && *reinterpret_cast<uint32_t*>(ImageBuff + 0x10) == UINT32_MAX)
                    {
                        auto _fetchAddr = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(ImageBuff + 0x08));

                        if (*reinterpret_cast<uint32_t*>(_fetchAddr + 0x10) == 0x02F02EB5)
                        {
                            auto _compareNoName = strncmp((const char*)(_fetchAddr + 0x14), "no_name", 0x07);

                            if (*reinterpret_cast<uint32_t*>(_fetchAddr + 0xB4) != UINT32_MAX)
                            {
                                *reinterpret_cast<uint32_t*>(_fetchAddr + 0xB0) = 0x0A;
                                *reinterpret_cast<uint32_t*>(_fetchAddr + 0xB4) = 0x01;

                                if (_compareNoName)
                                    *reinterpret_cast<uint64_t*>(_fetchAddr + 0xB8) = 0x00;

                                else
                                    *reinterpret_cast<uint64_t*>(_fetchAddr + 0xB8) = UINT64_MAX;
                            }
                        }
                    }

                    *CachePic = UINT32_MAX;
                }

                *Phase = 1;

                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
                string _constructPath = _fetchConfig & 0x0200 ? "itempic_2nd/item-%03d.imd" : (_fetchConfig & 0x0400 ? "itempic_3rd/item-%03d.imd" : "itempic/item-%03d.imd");

                sprintf(nameBuff, _constructPath.c_str(), _itempicID);

                if (!YS::FILE::GetSize(nameBuff))
                    sprintf(nameBuff, "itempic/item-%03d.imd", _itempicID);

                fprintf(stdout, "[YS::ITEMPIC::ReadImageThread] | Reading File: %s\n", nameBuff);

                YS::FILE::Read(nameBuff, ImageBuff);
                YI::IMAGE::CreateImage(ImageBuff);

                *CachePic = _itempicID;
                *Phase = 0;
                *UserNum = 0;

                return nullptr;
            }
		};
	}
}