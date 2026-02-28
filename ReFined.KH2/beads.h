#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

#include "pax.h"
#include "converter.h"
#include "limit.h"

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT VM
        {
            public:
                static uint32_t trap_obj_get_entry_id(uint32_t* bdvalue);
                static uint32_t trap_obj_effect_start_bind(uint32_t* bdvalue);
                static uint32_t trap_obj_effect_start_bind_other(uint32_t* bdvalue);
                static uint32_t trap_limit_motion_start(uint32_t* bdvalue);

                struct staticInitializer
                {
                    staticInitializer()
                    {
                        printf("======================================================\n");
                        printf("Handling hooks and redirections concerning YS::VM...\n\n");

                        vector<uint8_t> _absoluteInstructionJMP =
                        {
                            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        };

                        auto _constEffectStartBind = (uint64_t)trap_obj_effect_start_bind;
                        auto _effectStartBind_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x30\x8B\x59\x18\x4C\x8B\xF1\x8B\x79\x10\x8B\x71\x08\x8B\x09\xE8\x00\x00\x00\x00\x8B\x48\x04", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx");

                        printf("Fetched YS::trap_obj_effect_start_bind @ 0x%p\n", _effectStartBind_orig);

                        fill(_effectStartBind_orig, _effectStartBind_orig + 0x72, 0x90);

                        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constEffectStartBind, 0x08);
                        memcpy(_effectStartBind_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                        printf("Hooked YS::trap_obj_effect_start_bind [0x%p] to Re:Fined function @ 0x%p\n", _effectStartBind_orig, trap_obj_effect_start_bind);

                        auto _constEffectStartBindOther = (uint64_t)trap_obj_effect_start_bind_other;
                        auto _effectStartBindOther_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x30\x4C\x8B\xF1\x8B\x49\x10\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x41\x8B\x0E\x48\x8B\xD8\x41\x8B\x7E\x20\x41\x8B\x76\x18\x41\x8B\x6E\x08\xE8\x00\x00\x00\x00\x8B\x48\x04", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxx????xxx");

                        printf("Fetched YS::trap_obj_effect_start_bind_other @ 0x%p\n", _effectStartBindOther_orig);

                        fill(_effectStartBindOther_orig, _effectStartBindOther_orig + 0x8E, 0x90);

                        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constEffectStartBindOther, 0x08);
                        memcpy(_effectStartBindOther_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                        printf("Hooked YS::trap_obj_effect_start_bind_other [0x%p] to Re:Fined function @ 0x%p\n", _effectStartBindOther_orig, trap_obj_effect_start_bind_other);

                        auto _constObjGetEntryId = (uint64_t)trap_obj_get_entry_id;
                        auto _objGetEntryId_orig = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x8B\x09\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00\x89\x03\xC7\x43\x04\x40\x49\x4E\x54\x48\x83\xC4\x20\x5B\xC3\xCC\x48", "xxxxxxxxxxxx????xxxx????xxxx????xxxxxxxxxxxxxxxxx");

                        printf("Fetched YS::trap_obj_get_entry_id @ 0x%p\n", _objGetEntryId_orig);

                        fill(_objGetEntryId_orig, _objGetEntryId_orig + 0x2F, 0x90);

                        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constObjGetEntryId, 0x08);
                        memcpy(_objGetEntryId_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                        printf("Hooked YS::trap_obj_get_entry_id [0x%p] to Re:Fined function @ 0x%p\n", _objGetEntryId_orig, trap_obj_get_entry_id);

                        auto _constLimitMotionStart = (uint64_t)trap_limit_motion_start;
                        auto _limitMotionStart_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x8B\x79\x10\x48\x8B\xF1\x0F\x29\x74\x24\x20\xF3\x0F\x10\x71\x18\x8B\x49\x08\xE8\x00\x00\x00\x00\x8B\x48\x04\xE8\x00\x00\x00\x00\x8B\x0E\x48\x8B\xD8\xE8\x00\x00\x00\x00\x0F\x28\xDE\x44\x8B\xC7\x48\x8B\xD3\x48\x8B\xC8", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx????xxxxxx????xxxxxxxxxxxx");

                        printf("Fetched YS::trap_limit_motion_start @ 0x%p\n", _limitMotionStart_orig);

                        fill(_limitMotionStart_orig, _limitMotionStart_orig + 0x21, 0x90);

                        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constLimitMotionStart, 0x08);
                        memcpy(_limitMotionStart_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                        printf("Hooked YS::trap_limit_motion_start [0x%p] to Re:Fined function @ 0x%p\n", _limitMotionStart_orig, trap_limit_motion_start);

                        printf("\nSuccessfully handled YS::VM concerns.\n");
                        printf("======================================================\n\n");
                    }
                };

                static staticInitializer initialize;
        };
    }
}