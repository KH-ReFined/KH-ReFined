#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "file.h"
#include "area.h"
#include "region.h"
#include "memorymgr.h"
#include "cache_buff.h"

using namespace std;

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJENTRY
		{
        private:
            static bool _init()
            {
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

                auto get_mdlx_orig = FindSignature<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x85\xD2\x48\x8D\x79\x08", "xxxxxxxxxxxxxxxxx");
                auto get_apdx_orig = FindSignature<char*>("\x40\x57\x48\x83\xEC\x20\x0F\xB6\x41\x48", "xxxxxxxxxx");
                auto get_mset_orig = FindSignature<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x20\x4D\x85\xC0\x48\x8D\x3D", "xxxxxxxxxxxxxxxxxxxxxxxxxx");

                auto _readRequestSubMDLX = FindSignature<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxx?x????");
                auto _getCacheBuffStatusMDLX = FindSignature<char*>("\x4C\x8D\x46\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxx?x????");

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

                _instructionREPLACE[0] -= 0x01;
                _instructionREPLACE[2] -= 0x01;

                memcpy(_instructionREPLACE.data() + 0x07, &_calculateCacheBuffStatusMDLX, 0x04);
                memcpy(_getCacheBuffStatusMDLX, _instructionREPLACE.data(), _instructionREPLACE.size());

                memcpy(_instructionREPLACE.data() + 0x07, &_calculateCacheBuffStatusAPDX, 0x04);
                memcpy(_getCacheBuffStatusMDLX + 0xDA, _instructionREPLACE.data(), _instructionREPLACE.size());

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

                return true;
            }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif


		public:
			static inline char*(*Get)(uint32_t id) = FetchFunctionFromCall<char*(*)(uint32_t)>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x1E2);
			static inline uint32_t(*GetFriendPriority)(uint16_t id) = FetchFunctionFromCall<uint32_t(*)(uint16_t)>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x111);
            static inline char*(*ReadRequestWeapon)(int part, int hand, uint16_t id, int pri, int bank) = FetchFunctionFromCall<char*(*)(int, int, uint16_t, int, int)>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x169);

            static inline char* MDLX_WRITE_BUFFER = FetchRelativePointer<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????", 0x0E);
            static inline char* APDX_WRITE_BUFFER = FetchRelativePointer<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????", 0x0E) + 0x28;
            static inline char* MSET_WRITE_BUFFER = FetchRelativePointer<char*>("\x4D\x8D\x47\x08\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x36\xE8\x00\x00\x00\x00", "xxxxxxx????xxx????x????x????xxxxx????", 0x0E) + 0x50;

			static char* get_mdlx(char* objentryEntry, char* buff)
            {
                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
                string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.mdlx" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mdlx" : "obj/%s.mdlx");

                char* _mdlxName = objentryEntry + 0x08;
                char* _useBuff = !buff ? YS::OBJENTRY::MDLX_WRITE_BUFFER : buff;

                sprintf_s(_useBuff, 0x28, _constructPath.c_str(), _mdlxName);

                if (!YS::FILE::GetSize(_useBuff))
                    sprintf_s(_useBuff, 0x28, "obj/%s.mdlx", _mdlxName);

                return _useBuff;
            }

			static char* get_apdx(char* objentryEntry, char* buff)
            {
                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

                string _encodedRegion = _fetchConfig & 0x0004 ? "jp" :
                                       (_fetchConfig & 0x0008 ? "es" :
                                       (_fetchConfig & 0x0010 ? "de" :
                                       (_fetchConfig & 0x0020 ? "bg" : "us")));

                char* _apdxName = objentryEntry + 0x08;
                char* _useBuff = !buff ? YS::OBJENTRY::APDX_WRITE_BUFFER : buff;

                const char* _regionStr = !YS::REGION::Get() || YS::REGION::Get() == 0x07 ? "fm" : reinterpret_cast<char*>(*YS::REGION::Region);

                string _constructPath = _fetchConfig & 0x0200 ? "obj_2nd/%s.a.%s" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.a.%s" : "obj/%s.a.%s");

                if ((*(objentryEntry + 0x48) & 0x01) != 0x00)
                    return nullptr;

                sprintf_s(_useBuff, 0x28, _constructPath.c_str(), _apdxName, _encodedRegion.c_str());

                if (!YS::FILE::GetSize(_useBuff))
                    sprintf_s(_useBuff, 0x28, _constructPath.c_str(), _apdxName, _regionStr);

                if (!YS::FILE::GetSize(_useBuff))
                    sprintf_s(_useBuff, 0x28, _constructPath.c_str(), _apdxName, "us");

                if (!YS::FILE::GetSize(_useBuff))
                {
                    sprintf_s(_useBuff, 0x28, "obj/%s.a.%s", _apdxName, _encodedRegion.c_str());

                    if (!YS::FILE::GetSize(_useBuff))
                        sprintf_s(_useBuff, 0x28, "obj/%s.a.%s", _apdxName, _regionStr);

                    if (!YS::FILE::GetSize(_useBuff))
                        sprintf_s(_useBuff, 0x28, "obj/%s.a.us", _apdxName);
                }

                return _useBuff;
            }

			static char* get_mset(char* objentryEntry, uint32_t objectID, char* buff)
            {
                auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

                char* _mdlxName = objentryEntry + 0x08;
                char* _msetName = objentryEntry + 0x28;

                char* _useBuff = !buff ? YS::OBJENTRY::MSET_WRITE_BUFFER : buff;

                string _fetchMSET = _fetchConfig & 0x0200 ? "obj_2nd/%s.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s.mset" : "obj/%s.mset");
                string _fetchMEMO = _fetchConfig & 0x0200 ? "obj_2nd/%s_MEMO.mset" : (_fetchConfig & 0x0400 ? "obj_3rd/%s_MEMO.mset" : "obj/%s_MEMO.mset");

                if (!*_msetName)
                {
                    if ((objectID & 0x10000000) == 0x00)
                        return nullptr;

                    sprintf_s(_useBuff, 0x28, _fetchMEMO.c_str(), _mdlxName);

                    if (!YS::FILE::GetSize(_useBuff))
                        sprintf_s(_useBuff, 0x28, "obj/%s_MEMO.mset", _mdlxName);

                LABEL_14:
                    printf("[YS::OBJENTRY::get_mset] | Fulfilling MEMO_MSET request for: \"%s\"\n", _useBuff);
                    return _useBuff;
                }

                auto _fetchNameMSET = string(_msetName);
                _fetchNameMSET.resize(_fetchNameMSET.size() - 0x05);

                sprintf_s(_useBuff, 0x28, _fetchMSET.c_str(), _fetchNameMSET.c_str());

                if (!YS::FILE::GetSize(_useBuff))
                    sprintf_s(_useBuff, 0x28, "obj/%s.mset", _fetchNameMSET.c_str());

                if ((objectID & 0x10000000) != 0x00)
                {
                LABEL_12:
                    sprintf_s(_useBuff, 0x28, _fetchMEMO.c_str(), _fetchNameMSET.c_str());

                    if (!YS::FILE::GetSize(_useBuff))
                        sprintf_s(_useBuff, 0x28, "obj/%s_MEMO.mset", _fetchNameMSET.c_str());

                    goto LABEL_14;
                }

                if (objectID <= 0x31A)
                {
                    if (objectID > 0x318)
                        goto LABEL_10;

                LABEL_9:
                    if ((objectID & 0x20000000) == 0x00)
                        goto LABEL_13;

                    goto LABEL_10;
                }

                if (objectID != 0x03EE)
                    goto LABEL_9;

            LABEL_10:
                if (*(objentryEntry + 0x04) != 0x00 && YS::CACHE_BUFF::GetStatus(_useBuff) < 2)
                    goto LABEL_12;

            LABEL_13:
                return _useBuff;
            }
		};
	}
}