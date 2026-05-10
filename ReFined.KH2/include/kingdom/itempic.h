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
		public:
			using ReadImage_t = void(__fastcall*)(int _image);
			static ReadImage_t ReadImage;
			using FreeImageData_t = void(__fastcall*)(char* _imageData);
			static FreeImageData_t FreeImageData;

			static uint16_t* LoadedId;

			static char* ImageBuff;
			static uint16_t** ToLoadID;

			static uint32_t* Phase;
			static uint32_t* UserNum;
			static uint32_t* CachePic;
			static char* Task;

			static char* ReadImageThread(char* task);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning YS::ITEMPIC...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _readImageThread = (uint64_t)ReadImageThread;
                    auto _readImageThread_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx");

                    printf("Fetched YS::ITEMPIC::ReadImageThread @ 0x%p\n", _readImageThread_orig);

                    memset(_readImageThread_orig, 0x90, 0x115);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_readImageThread, 0x08);
                    memcpy(_readImageThread_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked YS::ITEMPIC::ReadImageThread [0x%p] to Re:Fined function @ 0x%p\n", _readImageThread_orig, ReadImageThread);

                    printf("\nSuccessfully handled YS::ITEMPIC concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}