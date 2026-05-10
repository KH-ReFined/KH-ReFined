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

using namespace std;

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJENTRY
		{
		public:
			using GetFriendPriority_t = uint32_t(*)(uint16_t id);
			static GetFriendPriority_t GetFriendPriority;

			using Get_t = char* (*)(uint32_t id);
			static Get_t Get;

			using ReadRequestWeapon_t = char* (*)(int part, int hand, uint16_t id, int pri, int bank);
			static ReadRequestWeapon_t ReadRequestWeapon;

            static char* MDLX_WRITE_BUFFER;
            static char* APDX_WRITE_BUFFER;
            static char* MSET_WRITE_BUFFER;

			static char* get_mdlx(char* objentryEntry, char* buff);
			static char* get_apdx(char* objentryEntry, char* buff);
			static char* get_mset(char* objentryEntry, uint32_t objectID, char* buff);

			struct staticInitializer
			{
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("Handling hooks and redirections concerning YS::OBJENTRY...\n\n");

                    MDLX_WRITE_BUFFER = ResolveRelativeAddress<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????", 0x0E);
                    APDX_WRITE_BUFFER = MDLX_WRITE_BUFFER + 0x28;
                    MSET_WRITE_BUFFER = APDX_WRITE_BUFFER + 0x28;

                    vector<uint8_t> _instructionREPLACE =
                    {
                        0x49, 0x8D, 0x0F,               // lea rcx, [rsi/r15]
                        0x48, 0x31, 0xD2,               // xor rdx, rdx
                        0xE8, 0x00, 0x00, 0x00, 0x00,   // call [someFunction]
                        0x48, 0x31, 0xC0                // xor rax, rax
                    };

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto get_mdlx_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x85\xD2\x48\x8D\x79\x08", "xxxxxxxxxxxxxxxxx");
                    auto get_apdx_orig = SignatureScan<char*>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx");
                    auto get_mset_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x20\x4D\x85\xC0\x48\x8D\x3D", "xxxxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched YS::OBJENTRY::get_mdlx @ 0x%p\n", get_mdlx_orig);
                    printf("Fetched YS::OBJENTRY::get_apdx @ 0x%p\n", get_apdx_orig);
                    printf("Fetched YS::OBJENTRY::get_mset @ 0x%p\n", get_mset_orig);

                    auto _readRequestSubMDLX = SignatureScan<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxx?x????");
                    auto _getCacheBuffStatusMDLX = SignatureScan<char*>("\x4C\x8D\x46\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxx?x????");

                    printf("Fetched YS::CACHE_BUFF::ReadRequestSub @ 0x%p\n", _readRequestSubMDLX);
                    printf("Fetched YS::OBJENTRY::GetCacheBuffStatus @ 0x%p\n", _getCacheBuffStatusMDLX);

                    uint32_t _calculateReadRequestMDLX = get_mdlx_orig - (_readRequestSubMDLX + 0x0B);
                    uint32_t _calculateCacheBuffStatusMDLX = get_mdlx_orig - (_getCacheBuffStatusMDLX + 0x0B);

                    uint32_t _calculateReadRequestAPDX = get_apdx_orig - (_readRequestSubMDLX + 0xCA + 0x0B);
                    uint32_t _calculateCacheBuffStatusAPDX = get_apdx_orig - (_getCacheBuffStatusMDLX + 0xDA + 0x0B);

                    fill(_readRequestSubMDLX, _readRequestSubMDLX + 0x56, 0x90);
                    fill(_getCacheBuffStatusMDLX, _getCacheBuffStatusMDLX + 0x56, 0x90);

                    fill(_readRequestSubMDLX + 0xCA, _readRequestSubMDLX + 0xCA + 0x62, 0x90);
                    fill(_getCacheBuffStatusMDLX + 0xDA, _getCacheBuffStatusMDLX + 0xDA + 0x62, 0x90);

                    memcpy(_instructionREPLACE.data() + 0x07, &_calculateReadRequestMDLX, 0x04);
                    memcpy(_readRequestSubMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());

                    memcpy(_instructionREPLACE.data() + 0x07, &_calculateReadRequestAPDX, 0x04);
                    memcpy(_readRequestSubMDLX + 0xCA, _instructionREPLACE.data(), _instructionREPLACE.size());

                    printf("Rewrote YS::CACHE_BUFF::ReadRequestSub [0x%p] to use YS::OBJENTRY::get_mdlx!\n", _readRequestSubMDLX);
                    printf("Rewrote YS::CACHE_BUFF::ReadRequestSub [0x%p] to use YS::OBJENTRY::get_apdx!\n", _readRequestSubMDLX + 0xCA);

                    _instructionREPLACE[0] -= 0x01;
                    _instructionREPLACE[2] -= 0x01;

                    memcpy(_instructionREPLACE.data() + 0x07, &_calculateCacheBuffStatusMDLX, 0x04);
                    memcpy(_getCacheBuffStatusMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());

                    memcpy(_instructionREPLACE.data() + 0x07, &_calculateCacheBuffStatusAPDX, 0x04);
                    memcpy(_getCacheBuffStatusMDLX + 0xDA, _instructionREPLACE.data(), _instructionREPLACE.size());

                    printf("Rewrote YS::OBJENTRY::GetCacheBuffStatus [0x%p] to use YS::OBJENTRY::get_mdlx!\n", _getCacheBuffStatusMDLX);
                    printf("Rewrote YS::OBJENTRY::GetCacheBuffStatus [0x%p] to use YS::OBJENTRY::get_apdx!\n", _getCacheBuffStatusMDLX + 0xDA);

                    fill(get_mdlx_orig, get_mdlx_orig + 0x71, 0x90);
                    fill(get_apdx_orig, get_apdx_orig + 0x92, 0x90);
                    fill(get_mset_orig, get_mset_orig + 0xE3, 0x90);

                    for (int i = 0; i < 3; i++)
                    {
                        auto _constFunction = i == 1 ? (uint64_t)YS::OBJENTRY::get_apdx : (i == 2 ? (uint64_t)YS::OBJENTRY::get_mset : (uint64_t)YS::OBJENTRY::get_mdlx);
                        auto _constWrite = i == 1 ? get_apdx_orig : (i == 2 ? get_mset_orig : get_mdlx_orig);

                        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
                        memcpy(_constWrite, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
                    }

                    printf("Hooked YS::OBJENTRY::get_mdlx [0x%p] to Re:Fined function @ 0x%p\n", get_mdlx_orig, YS::OBJENTRY::get_mdlx);
                    printf("Hooked YS::OBJENTRY::get_apdx [0x%p] to Re:Fined function @ 0x%p\n", get_apdx_orig, YS::OBJENTRY::get_apdx);
                    printf("Hooked YS::OBJENTRY::get_mset [0x%p] to Re:Fined function @ 0x%p\n", get_mset_orig, YS::OBJENTRY::get_mset);

                    printf("\nSuccessfully handled YS::OBJENTRY concerns.\n\n");
                }
			};

			static staticInitializer initialize;
		};
	}
}