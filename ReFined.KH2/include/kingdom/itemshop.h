#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define _CRT_SECURE_NO_WARNINGS

#include "memorymgr.h"
#include "panacea_alloc.h"
#include "member.h"

extern "C"
{
	namespace OvlShop
	{
		class DLL_EXPORT ItemShop
		{
		public:
			static void GetFaceFileNmae(int part, char* buffer);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning OvlShop::ItemShop...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _getFileNameFunc = (uint64_t)GetFaceFileNmae;
                    auto _getFileNameOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xDA\x8B\xF9\x48\x8B\xCB\x48\x8D\x15\x00\x00\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????x????");

                    printf("Fetched OvlShop::ItemShop::GetFaceFileNmae @ 0x%p\n", _getFileNameOriginal);

                    memset(_getFileNameOriginal, 0x90, 0xFA);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_getFileNameFunc, 0x08);
                    memcpy(_getFileNameOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked OvlShop::ItemShop::GetFaceFileNmae [0x%p] to Re:Fined function @ 0x%p\n", _getFileNameOriginal, GetFaceFileNmae);

                    printf("\nSuccessfully handled OvlShop::ItemShop concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}