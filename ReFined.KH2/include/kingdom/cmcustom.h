#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <set>
#include "memorymgr.h"
#include "partyinfo.h"
#include "selhist.h"
#include "menubase.h"
#include "cmcomm.h"
#include "cmtop.h"
#include "message.h"
#include "hookintro.h"
#include "layout.h"
#include "scrollbar.h"
#include "image_freeze.h"
#include "iteminfo.h"
#include "sequence.h"
#include "command_elem.h"
#include "cmenuhelp.h"
#include "friendsaveram.h"
#include "panacea_alloc.h"
#include "sprite.h"
#include "menuutil.h"
#include "magic.h"
#include "item_table.h"
#include "select.h"

extern "C"
{

    namespace Tz
    {
        class DLL_EXPORT CmCustom
        {
        public:
            static void(*CreateTopList)();

            static char* (*GetFriendInfo)(int type, int num);

            static int* m_pri;
            static char** m_ListInfo;

            static short* s_ButtonIcon;

            static char** m_PartyInfo;

            static char* s_PlayerType;
            static char* s_FriendType;

            static char* s_SelSeq;

            static char** LS_52_type;
            static char** LS_45_type;
            static char** LS_62_type;
            static char** LS_103_type;

            static char* LS_103_s_Seq;
            static int* LS_57_s_MessTbl;

            static char* LS_KH1F_Shortcuts;

            static void SetupTop();
            static void UpdateTopList();
            static void GetListInfo(int num);
            static int CurPos2CustomType(int pos);
            static int GetCustomItemNum(int type);
            static void MakeListInfo2ItemMess();
            static void UpdateHelpMess();
            static void UpdateCustomList();
            static void SetupCustom();
            static void ChangeCustomInfo();
            static void UpdateIndiCustomList();

            // These did have their typos in the symbols.
            static void ChageAbility(int pos);
            static void ChageMpDrive();
            static void ChangeAutoReplenishment();
            static void ChangePartyBehavior(int pos);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning Tz::CmCustom...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    uint64_t _changePartyAiFunction = reinterpret_cast<uint64_t>(ChangePartyBehavior);
                    auto _changePartyAiOriginal = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x8B\xD9\xB9\x19\x00\x00\x00\xE8", "xxxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::ChangePartyBehavior @ 0x%p\n", _changePartyAiOriginal);

                    fill(_changePartyAiOriginal, _changePartyAiOriginal + 0x75, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_changePartyAiFunction, 0x08);
                    memcpy(_changePartyAiOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::ChangePartyBehavior [0x%p] to Re:Fined function @ 0x%p\n", _changePartyAiOriginal, ChangePartyBehavior);

                    uint64_t _changeAbilityFunction = reinterpret_cast<uint64_t>(ChageAbility);
                    auto _changeAbilityOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x8B\xF9\xE8\x00\x00\x00\x00\xB9\x19\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxxx");

                    printf("Fetched Tz::CmCustom::ChageAbility @ 0x%p\n", _changeAbilityOriginal);

                    fill(_changeAbilityOriginal, _changeAbilityOriginal + 0xFA, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_changeAbilityFunction, 0x08);
                    memcpy(_changeAbilityOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::ChageAbility [0x%p] to Re:Fined function @ 0x%p\n", _changeAbilityOriginal, ChageAbility);

                    uint64_t _changeCustomInfoFunction = reinterpret_cast<uint64_t>(ChangeCustomInfo);
                    auto _changeCustomInfoOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\xB9\x1A\x00\x00\x00\xE8", "xxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::ChangeCustomInfo @ 0x%p\n", _changeCustomInfoOriginal);

                    fill(_changeCustomInfoOriginal, _changeCustomInfoOriginal + 0x160, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_changeCustomInfoFunction, 0x08);
                    memcpy(_changeCustomInfoOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::ChangeCustomInfo [0x%p] to Re:Fined function @ 0x%p\n", _changeCustomInfoOriginal, ChangeCustomInfo);
                    
                    uint64_t _posToTypeFunction = reinterpret_cast<uint64_t>(CurPos2CustomType);
                    auto _posToTypeOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xF9\xB9\x19\x00\x00\x00\xE8", "xxxxxxxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::CurPos2CustomType @ 0x%p\n", _posToTypeOriginal);

                    fill(_posToTypeOriginal, _posToTypeOriginal + 0xBA, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_posToTypeFunction, 0x08);
                    memcpy(_posToTypeOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::CurPos2CustomType [0x%p] to Re:Fined function @ 0x%p\n", _posToTypeOriginal, CurPos2CustomType);

                    uint64_t _setupCustomFunction = reinterpret_cast<uint64_t>(SetupCustom);
                    auto _setupCustomOriginal = SignatureScan<char*>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx");

                    printf("Fetched Tz::CmCustom::SetupCustom @ 0x%p\n", _setupCustomOriginal);

                    fill(_setupCustomOriginal, _setupCustomOriginal + 0x555, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_setupCustomFunction, 0x08);
                    memcpy(_setupCustomOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::SetupCustom [0x%p] to Re:Fined function @ 0x%p\n", _setupCustomOriginal, SetupCustom);
                    
                    uint64_t _updateCustomListFunction = reinterpret_cast<uint64_t>(UpdateCustomList);
                    auto _updateCustomListOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x60\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x58", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxx");

                    printf("Fetched Tz::CmCustom::UpdateCustomList @ 0x%p\n", _updateCustomListOriginal);

                    fill(_updateCustomListOriginal, _updateCustomListOriginal + 0x415, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_updateCustomListFunction, 0x08);
                    memcpy(_updateCustomListOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::UpdateCustomList [0x%p] to Re:Fined function @ 0x%p\n", _updateCustomListOriginal, UpdateCustomList);

                    uint64_t _updateHelpMessageFunction = reinterpret_cast<uint64_t>(UpdateHelpMess);
                    auto _updateHelpMessageOriginal = SignatureScan<char*>("\x40\x53\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x28\x48\x89\x7C\x24\x60\x4C\x89\x74\x24\x20\xE8", "xxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::UpdateHelpMess @ 0x%p\n", _updateHelpMessageOriginal);

                    fill(_updateHelpMessageOriginal, _updateHelpMessageOriginal + 0x317, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_updateHelpMessageFunction, 0x08);
                    memcpy(_updateHelpMessageOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::UpdateHelpMess [0x%p] to Re:Fined function @ 0x%p\n", _updateHelpMessageOriginal, UpdateHelpMess);

                    uint64_t _makeListMessageFunction = reinterpret_cast<uint64_t>(MakeListInfo2ItemMess);
                    auto _makeListMessageOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x63\xD8\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x8B\xF0\x83\xFB\x19\x75\x0E", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxx????xxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::MakeListInfo2ItemMess @ 0x%p\n", _makeListMessageOriginal);

                    fill(_makeListMessageOriginal, _makeListMessageOriginal + 0x3EA, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_makeListMessageFunction, 0x08);
                    memcpy(_makeListMessageOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::MakeListInfo2ItemMess [0x%p] to Re:Fined function @ 0x%p\n", _makeListMessageOriginal, MakeListInfo2ItemMess);

                    uint64_t _updateTopFunction = reinterpret_cast<uint64_t>(UpdateTopList);
                    auto _updateTopOriginal = ResolveRelativeAddress<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F);

                    printf("Fetched Tz::CmCustom::UpdateTopList @ 0x%p\n", _updateTopOriginal);

                    fill(_updateTopOriginal, _updateTopOriginal + 0x2B6, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_updateTopFunction, 0x08);
                    memcpy(_updateTopOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::UpdateTopList [0x%p] to Re:Fined function @ 0x%p\n", _updateTopOriginal, UpdateTopList);

                    uint64_t _setupTopFunction = reinterpret_cast<uint64_t>(SetupTop);
                    auto _setupTopOriginal = SignatureScan<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::SetupTop @ 0x%p\n", _setupTopOriginal);

                    fill(_setupTopOriginal, _setupTopOriginal + 0x235, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_setupTopFunction, 0x08);
                    memcpy(_setupTopOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::SetupTop [0x%p] to Re:Fined function @ 0x%p\n", _setupTopOriginal, SetupTop);

                    uint64_t _getInfoFunction = reinterpret_cast<uint64_t>(GetListInfo);
                    auto _getInfoOriginal = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched Tz::CmCustom::GetListInfo @ 0x%p\n", _getInfoOriginal);

                    fill(_getInfoOriginal, _getInfoOriginal + 0x65C, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_getInfoFunction, 0x08);
                    memcpy(_getInfoOriginal, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Tz::CmCustom::GetListInfo [0x%p] to Re:Fined function @ 0x%p\n", _getInfoOriginal, GetListInfo);

                    printf("\nSuccessfully handled Tz::CmCustom concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}
