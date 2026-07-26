#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <unordered_set>
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
#include "menusound.h"
#include "cmtutorial.h"

extern "C"
{
    namespace Tz
    {
        class DLL_EXPORT CmCustom
        {
        public:
            static inline bool CAN_ALTER_KH1F = false;

            static inline void(*SetupCustomList)() = FindSignature<void(*)()>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8\x00\x00\x00\x00\x33\xD2", "xxxxxxxxxxxxxxxxxxxxx????xx");
            static inline bool(*SelCheck)(int) = FindSignature<bool(*)(int)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x8B\xF9\xE8\x00\x00\x00\x00\x83\xF8\x19", "xxxxxxxxxxxxxxxxxx????xxx");
            static inline void(*ShowErrorTakeOff)() = FindSignature<void(*)()>("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x33\xD2\x88\x05\x00\x00\x00\x00\xB9\x6A\x84\x00\x00\xC6\x05", "xxxxx????xxxx????xxxxxxx");
            
            static inline void(*CreateTopList)() = FetchFunctionFromCall<void(*)()>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0204);
            static inline void(*ChageMpDrive)() = FindSignature<void(*)()>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x83\xF8\x1A\x75\x0E\x48\x8B\x0D", "xxxxxxxxxxxxxxxxxxxxx????xxxxxxxx");

            static inline void(*SetIndiCustomDefaultPos)() = FindSignature<void(*)()>("\x48\x89\x5C\x24\x20\x56\x48\x83\xEC\x30\x48\x89\x6C\x24\x40\xB9\x1A\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxx");
            static inline void(*UpdateIndiCustomList)() = FindSignature<void(*)()>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x40\xE8", "xxxxxxxxxxxx");

            static inline char* DrawItemAILv = FindSignature<char*>("\x40\x55\x56\x57\x48\x81\xEC\xD0\x00\x00\x00\x48\x8B\x05", "xxxxxxxxxxxxxx");

            static inline char* (*GetFriendInfo)(int type, int num) = FindSignature<char* (*)(int, int)>("\x48\x89\x5C\x24\x08\x45\x33\xDB\x8B\xDA\x45\x8B\xD3\x45\x8B\xCB", "xxxxxxxxxxxxxxxx");

            static inline int* m_pri = FetchRelativePointer<int*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0027);
            static inline char** m_ListInfo = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x004D);

            static inline short* s_ButtonIcon = FetchAbsolutePointer<short*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x63\xD8\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x8B\xF0\x83\xFB\x19\x75\x0E", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxx????xxxxxxxxxxxxx", 0x0172);

            static inline char** m_PartyInfo = FetchRelativePointer<char**>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0063);

            static inline char* s_PlayerType = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0046);
            static inline char* s_FriendType = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x003C);
            static inline char* s_ChgSelCharaNext = FetchRelativePointer<char*>("\x48\x83\xEC\x28\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x74\x15\xBA\x98\x00\x00\x00\xE8\x00\x00\x00\x00\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC6\x05", "xxxxxxx????xxxxxxxxxxx????xxx????xxxxxx", 0x27) + 0x01;

            static inline char* s_SelSeq = FetchRelativePointer<char*>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", 0x02FD);

            static inline char** LS_52_type = FetchRelativePointer<char**>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x006F) + 0x02;
            static inline char** LS_45_type = FetchRelativePointer<char**>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x006F) + 0x01;
            static inline char** LS_62_type = FetchRelativePointer<char**>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x006F) - 0x01;

            static inline char** LS_103_type = FetchRelativePointer<char**>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x006F);
            static inline char* LS_103_s_Seq = FetchRelativePointer<char*>(FetchRelativePointer<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x014D);

            static inline int* LS_57_s_MessTbl = FetchAbsolutePointer<int*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x63\xD8\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x8B\xF0\x83\xFB\x19\x75\x0E", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxx????xxxxxxxxxxxxx", 0x028C);

            static inline char* LS_KH1F_Shortcuts = FindSignature<char*>("\xBA\x02\xBD\x02\xC0\x02\xAB\x02\x00\x00\x7A\x44\x00\x00", "xxxxxxxxxxxxxx");

            static int CurPos2CustomType(int pos)
            {
                *Tz::CmCustom::LS_52_type = Tz::CmTop::GetCurPos(25) == 0x00 ? Tz::CmCustom::s_PlayerType : (Tz::CmTop::GetCurPos(25) == 0x01 && Tz::CmCustom::CheckKH1Form() ? Tz::CmCustom::s_PlayerType - 0x04 : Tz::CmCustom::s_FriendType);

                auto _fetchItemCalc = 0x00;

                for (int i = 0; i < 3; i++)
                {
                    auto _fetchItemType = *(*Tz::CmCustom::LS_52_type + i);
                    auto _fetchItemNum = Tz::CmCustom::GetCustomItemNum(_fetchItemType);

                    _fetchItemCalc += _fetchItemNum;

                    if (pos < _fetchItemCalc)
                        return _fetchItemType;
                }

                return -1;
            }

            static int GetCustomItemNum(int type)
            {
                auto _fetchListInfo = *Tz::CmCustom::m_ListInfo;

                if (type)
                {
                    switch (type)
                    {
                    case 0x01:
                        return *_fetchListInfo;
                    case 0x02:
                        return _fetchListInfo[1];
                    case 0x03:
                        return 1;
                    case 0x04:
                        return _fetchListInfo[2];
                    }
                }

                else
                    return 0x04;
            }

            static void ChageAbility(int pos)
            {
                auto _fetchMode = Tz::MenuBase::GetMode();
                auto _fetchFriendInfo = Tz::CmCustom::GetFriendInfo(4, -1);
                auto _fetchSelectPos = Tz::CmTop::GetSelectPos(25);
                auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, _fetchSelectPos && Tz::CmCustom::CheckKH1Form() ? _fetchSelectPos - 1 : _fetchSelectPos) + 0x250);

                auto _fetchSelectAbility = Tz::CmTop::GetSelectPos(26);

                if (_fetchMode == 26 || _fetchMode == 27)
                    _fetchSelectAbility = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                auto _fetchIndex = kn::FriendSaveRam::item2index(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectAbility));
                auto _friendIterator = 0x00;

                if (pos < 0x00)
                {
                    while (*(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectAbility + 0x03) != *(_fetchFriendInfo + 0x08 * _friendIterator + 0x05))
                    {
                        _friendIterator++;

                        if (_friendIterator >= 0x03)
                            break;
                    }

                    if (_friendIterator < 0x03)
                        pos = _friendIterator;

                    if (++pos >= 0x03)
                        pos = 0x00;
                }

                auto _fetchAbility = *(_fetchFriendInfo + 0x08 * pos + 0x05);
                auto _convertSheet = reinterpret_cast<char*>(PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet));

                *(_convertSheet + _fetchIndex + 0x00F5) = _fetchAbility;
                *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectAbility + 0x03) = _fetchAbility;
            }

            static void ChangeAutoReplenishment()
            {
                auto _fetchMode = Tz::MenuBase::GetMode();
                auto _fetchCurrentPos = Tz::CmTop::GetSelectPos(26);
                auto _fetchTopSelectPos = Tz::CmTop::GetSelectPos(25);

                auto _isKH1Form = _fetchTopSelectPos == 0x01 && Tz::CmCustom::CheckKH1Form();

                if (_fetchMode == 26 || _fetchMode == 27 || _fetchMode == 28)
                    _fetchCurrentPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                if (_fetchTopSelectPos && !_isKH1Form)
                    _fetchCurrentPos--;

                auto _fetchItem = *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x08);
                auto _fetchFlagCheck = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) == 0x00;

                auto _fetchFinalFlag = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                if (_fetchTopSelectPos && !_isKH1Form)
                {
                    if (_fetchMode != 31)
                    {
                        auto _fetchReplenishFlag = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A);
                        auto _fetchBehaviorFlag = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0B);

                        _fetchFinalFlag = (_fetchReplenishFlag | _fetchBehaviorFlag) + 0x01;

                        if (_fetchFinalFlag >= 0x04)
                            _fetchFinalFlag = 0x00;
                    }

                    *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) = _fetchFinalFlag & 0x01;
                    *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0B) = _fetchFinalFlag & 0x02;

                    _fetchFlagCheck = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) != 0x00;
                }

                auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, (_fetchTopSelectPos && Tz::CmCustom::CheckKH1Form()) ? _fetchTopSelectPos - 1 : _fetchTopSelectPos) + 0x250);
                auto _convertSheet = reinterpret_cast<char*>(PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet));

                auto _fetchPartyCount = *(_convertSheet + 0x12);

                for (int i = 0; i < _fetchPartyCount; i++)
                {
                    auto _fetchCurrentItemPtr = _convertSheet + 0x44 + 0x02 * i;

                    auto _fetchCurrItem = *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr - 0x10);
                    auto _fetchCurrReplenish = *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr);

                    if (_fetchCurrItem == _fetchItem || _fetchCurrReplenish == _fetchItem)
                        *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr) = _fetchFlagCheck ? _fetchItem : 0x00;
                }

                if (_fetchTopSelectPos && !_isKH1Form)
                    *(_convertSheet + kn::FriendSaveRam::item2index(_fetchItem) + 0x0F5) = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0B);

                if (_fetchMode == 26 || _fetchMode == 27 || _fetchMode == 28)
                {
                    auto _fetchEntryCount = *(*Tz::CmCustom::m_ListInfo + 0x04);

                    Tz::CmCustom::GetListInfo(_fetchTopSelectPos);
                    Tz::CmCustom::MakeListInfo2ItemMess();

                    auto _fetchEntryCountNew = *(*Tz::CmCustom::m_ListInfo + 0x04);

                    if (_fetchEntryCountNew != _fetchEntryCount)
                    {
                        if (_fetchTopSelectPos && !_isKH1Form)
                            _fetchEntryCountNew++;

                        auto _fetchCurrent = *reinterpret_cast<uint32_t*>(*Tz::CmTop::m_MenuPtr);
                        auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);
                        auto _fetchItemMax = *reinterpret_cast<uint16_t*>(Tz::CmTop::m_MenuPtr + 0x04);

                        auto _shouldUpdateScrollBar = false;

                        if (_fetchCurrent)
                        {
                            if (_fetchSelectTop == 0x00 || (_fetchSelectTop + _fetchItemMax) <= _fetchEntryCountNew)
                                _fetchCurrent--;

                            _shouldUpdateScrollBar = true;
                        }

                        else
                        {
                            if (_fetchSelectTop != 0x00)
                                _fetchSelectTop--;

                            _shouldUpdateScrollBar = true;
                        }

                        if (_shouldUpdateScrollBar)
                        {
                            Tz::ScrollBar::SetMax(*Tz::CmTop::m_SclBar, _fetchEntryCountNew);
                            Tz::ScrollBar::SetPos(*Tz::CmTop::m_SclBar, _fetchSelectTop);
                            Tz::Select::SetCurrent(*Tz::CmTop::m_MenuPtr, _fetchCurrent, 0, -1, -1);
                            Tz::Select::SetSelectTop(*Tz::CmTop::m_MenuPtr, _fetchSelectTop);
                            Tz::Select::SetSelectMax(*Tz::CmTop::m_MenuPtr, _fetchEntryCountNew);
                        }
                    }
                }
            }

            static void ChangePartyBehavior(int pos)
            {
                auto _fetchSelectPos = Tz::CmTop::GetSelectPos(25);
                auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, _fetchSelectPos && Tz::CmCustom::CheckKH1Form() ? _fetchSelectPos - 1 : _fetchSelectPos) + 0x250);
                auto _convertSheet = reinterpret_cast<char*>(PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet));

                if (pos >= 0)
                {
                    *(_convertSheet + 0x00F4) = pos;
                    *(*Tz::CmCustom::m_ListInfo + 0x03) = pos;
                }

                else
                {
                    auto _behaviorPtr = _convertSheet + 0x00F4;
                    *_behaviorPtr += 1;

                    if (*_behaviorPtr >= 0x06)
                        *_behaviorPtr = 0;

                    *(*Tz::CmCustom::m_ListInfo + 0x03) = *_behaviorPtr;
                }
            }

            static void SetupCustom()
            {
                uint16_t IndiCustomSelSeq[] = { 0x00CA, 0x00C1, 0x00B8, 0x00A6, 0x009D };
                uint16_t IndiCustomTitleSeq[] = { 0x00CE, 0x00C5, 0x00BC, 0x00AA, 0x00A1, 0x0000 };
                uint16_t IndiCustomBaseSeq[] = { 0x00C6, 0x00C8, 0x00C7, 0x00BD, 0x00BF, 0x00BE, 0x00B4, 0x00B6, 0x00B5, 0x00A2, 0x00A4, 0x00A3, 0x0099, 0x009B, 0x009A, 0x0000 };

                int _fetchInfoMax = -1;

                auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                auto _fetchMode = Tz::MenuBase::GetMode();
                auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);

                auto _currSeqdPtr = Tz::CmCustom::s_SelSeq + 0x04;

                while (_currSeqdPtr < reinterpret_cast<char*>(Tz::CmCustom::LS_62_type) + 0x04)
                {
                    *reinterpret_cast<uint16_t*>(_currSeqdPtr - 0x04) = IndiCustomSelSeq[_fetchCustomType];
                    *reinterpret_cast<uint16_t*>(_currSeqdPtr - 0x02) = IndiCustomSelSeq[_fetchCustomType];
                    *reinterpret_cast<uint16_t*>(_currSeqdPtr) = IndiCustomSelSeq[_fetchCustomType];
                    *reinterpret_cast<uint16_t*>(_currSeqdPtr + 0x02) = IndiCustomSelSeq[_fetchCustomType];

                    _currSeqdPtr += 0x04;
                }

                auto _isKH1Form = Tz::CmTop::GetSelectPos(25) == 0x01 && Tz::CmCustom::CheckKH1Form();

                if (_fetchCustomType)
                {
                    switch (_fetchCustomType)
                    {
                    case 0x01:
                        _fetchInfoMax = 0x02;
                        break;
                    case 0x02:
                    {
                        _fetchInfoMax = 0x02;

                        if (Tz::CmTop::GetSelectPos(25))
                            _fetchInfoMax = 0x04;
                    } break;

                    case 0x03:
                        _fetchInfoMax = 0x06;
                        break;
                    default:
                        _fetchInfoMax = 0x03;
                        break;
                    }
                }

                else
                {
                    memset(*Tz::CmTop::m_ItemInfo, 0x00, 0x30);

                    if (!_isKH1Form)
                        Tz::ItemInfo::SetItemInfo(*Tz::CmTop::m_ItemInfo, 0x00, 0x0A);

                    else
                    {
                        memcpy(*Tz::CmTop::m_ItemInfo, "\x04", 0x01);

                        memcpy(*Tz::CmTop::m_ItemInfo + 0x04, "\x39\x02", 0x02);
                        memcpy(*Tz::CmTop::m_ItemInfo + 0x0A, "\x3A\x02", 0x02);
                        memcpy(*Tz::CmTop::m_ItemInfo + 0x10, "\x3B\x02", 0x02);
                        memcpy(*Tz::CmTop::m_ItemInfo + 0x16, "\x3C\x02", 0x02);
                    }

                    if (!_isKH1Form)
                        Tz::ItemInfo::AddKnowItemInfo(*Tz::CmTop::m_ItemInfo);

                    Tz::ItemInfo::MakeEmptyMsg(*Tz::CmTop::m_ItemInfo, -1);
                    Tz::ItemInfo::SortItemInfo(*Tz::CmTop::m_ItemInfo);

                    _fetchInfoMax = *reinterpret_cast<int*>(*Tz::CmTop::m_ItemInfo);
                    Tz::ItemInfo::MakeCommandMsg(*Tz::CmTop::m_ItemInfo, 0, _fetchInfoMax);

                    if (_isKH1Form)
                        for (int i = 0; i < 0x04; i++)
                            *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_ItemInfo + 0x0784 + 0x10 + (0x50 * i)) = 0xCF09;

                    _fetchInfoMax += 0x01;
                }

                Tz::SelHist::Create(0x02, -1);
                auto _fetchPriority = *Tz::CmCustom::m_pri;

                Tz::MenuUtil::CreateSprt(_fetchBuffer, _fetchPriority, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, IndiCustomBaseSeq[3 * _fetchCustomType], IndiCustomBaseSeq[3 * _fetchCustomType + 0x02], 0, 0);
                Tz::MenuUtil::CreateMess(_fetchBuffer + 0x07E0, _fetchPriority + 0x01, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, IndiCustomTitleSeq[_fetchCustomType], -1, -1, 0, 0);

                *reinterpret_cast<uint32_t*>(_fetchBuffer + 0x07F0) &= ~0x10;
                Tz::MenuUtil::SetSprtParent(_fetchBuffer + 0x07E0, _fetchBuffer);

                auto _fetchFontColor = !_isKH1Form ? Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x08), 1, _fetchCustomType == 1) : 0x00;
                auto _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);

                Tz::MenuUtil::CreateMess(_fetchBuffer + 0x7660, _fetchPriority + 0x02, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, _fetchFontColorSeq, -1, -1, 0, 0);

                *reinterpret_cast<char**>(_fetchBuffer + 0x7858) = _fetchBuffer + (_fetchSelectPos + 0x6E7) * 0x20;
                Tz::MenuUtil::SetSprtParent(_fetchBuffer + 0x7660, _fetchBuffer + 0x07E0);

                *reinterpret_cast<uint32_t*>(_fetchBuffer + 0x7670) &= ~0x10;
                *reinterpret_cast<uint32_t*>(_fetchBuffer + 0x7670) &= ~0x08;

                auto _fetchSizePage = _fetchInfoMax > 0x08 ? 0x08 : _fetchInfoMax;

                _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(0x00);

                for (int i = 0; i < _fetchSizePage; i++)
                {
                    auto _sideObject = _fetchBuffer + 0x7880 + 0x220 * i;

                    Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, i, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(Tz::CmCustom::s_SelSeq), reinterpret_cast<uint16_t*>(Tz::CmCustom::s_SelSeq + 0x08));

                    auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i);
                    auto _paramCr = YI::SEQUENCE::GetParamCr(_fetchItemPtr + 0x20);

                    Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, i, 0, _paramCr);

                    *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x10) &= ~0x10;

                    if (_fetchMode == 31 && Tz::CmTop::GetSelectPos(25) && !_isKH1Form)
                        *reinterpret_cast<char**>(Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i) + 0x220) = Tz::CmCustom::DrawItemAILv;

                    Tz::MenuUtil::SetSprtParent(_fetchItemPtr, _fetchBuffer);
                    Tz::MenuUtil::CreateMess(_sideObject, _fetchPriority + 0x02, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, _fetchFontColorSeq, -1, -1, 0, 0);
                    Tz::MenuUtil::SetSprtParent(_sideObject, _fetchItemPtr);

                    *reinterpret_cast<uint32_t*>(_sideObject + 0x10) &= ~0x10;
                    *reinterpret_cast<uint32_t*>(_sideObject + 0x10) &= ~0x08;
                }

                auto _fetchLayout = *Tz::CmTop::m_Lay + 0x50;
                auto _fetchSeqUnit = YI::LAYOUT::GetSequenceUnit(_fetchLayout, *Tz::CmTop::m_SeqUnit);
                auto _fetchImage = YI::IMAGE_FREEZE::GetImage(*reinterpret_cast<char**>(_fetchLayout + 0x48), *Tz::CmTop::m_ImgUnit);
                Tz::ScrollBar::Create(*Tz::CmTop::m_SclBar, _fetchPriority + 0x01, _fetchSeqUnit, _fetchImage, Tz::CmTop::GetScrBarSeqTbl(), _fetchInfoMax, _fetchSizePage, _fetchBuffer);

                Tz::Select::SetPageMode(*Tz::CmTop::m_MenuPtr, 0x01);
                *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x1C) = _fetchSizePage;

                if (_fetchInfoMax > 0x08)
                    Tz::Select::SetSelectMax(*Tz::CmTop::m_MenuPtr, _fetchInfoMax);

                Tz::CmCustom::SetIndiCustomDefaultPos();

                Tz::MenuBase::SetSMode(0);
                Tz::CmCustom::UpdateIndiCustomList();
            }

            static void UpdateCustomList()
            {
                int _itemNumArray[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                uint16_t _listSelSeq[] = { 0x0108, 0x0107, 0x0106, 0x00FA, 0x00F8, 0x0000, 0x0000, 0x0000 };
                uint16_t _listSubSeq[] = { 0x011A, 0x0119, 0x0118, 0x0115, 0x0114, 0x0000 };
                uint16_t _listSubMsg[] = { 0x847D, 0x8481, 0x8482, 0x8483, 0x8484, 0x0000 };

                uint16_t _listShortcutSubMsg[] = { 0x847D, 0x847E, 0x847F, 0x8480 };

                auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);

                auto _fetchCurPos = Tz::CmTop::GetCurPos(25);
                auto _isKH1Form = _fetchCurPos == 0x01 && Tz::CmCustom::CheckKH1Form();

                *Tz::CmCustom::LS_45_type = _fetchCurPos == 0x00 ? Tz::CmCustom::s_PlayerType : (_fetchCurPos == 0x01 && Tz::CmCustom::CheckKH1Form() ? Tz::CmCustom::s_PlayerType - 0x04 : Tz::CmCustom::s_FriendType);

                if (_isKH1Form && *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x16) != 0x04)
                {
                    Tz::Select::SetSelectMax(*Tz::CmTop::m_MenuPtr, 0x04);

                    Tz::ScrollBar::SetMax(*Tz::CmTop::m_SclBar, 0x01);
                    Tz::ScrollBar::SetPos(*Tz::CmTop::m_SclBar, 0x01);
                }

                else
                    Tz::ScrollBar::SetPos(*Tz::CmTop::m_SclBar, _fetchSelectTop);

                auto _fetchItemMax = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x04);
                auto _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(0);

                for (int i = 0; i < _fetchItemMax; i++)
                {
                    auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i);
                    dk::Sprite::setNum(_fetchItemPtr, *Tz::CmTop::m_DummySeq);
                }

                for (int i = 0; i < 0x09; i++)
                {
                    dk::Sprite::setNum(_fetchBuffer + 0x1060 + 0x0220 * i + 0x6600, _fetchFontColorSeq);
                    *reinterpret_cast<char**>(_fetchBuffer + 0x1258 + 0x0220 * i + 0x6600) = nullptr;

                    dk::Sprite::setNum(_fetchBuffer + 0x1060 + 0x0220 * i, *Tz::CmTop::m_DummySeq);
                    *reinterpret_cast<char**>(_fetchBuffer + 0x1258 + 0x0220 * i) = nullptr;

                    dk::Sprite::setNum(_fetchBuffer + 0xA960 + 0x01F8 * i, *Tz::CmTop::m_DummySeq);
                }

                for (int i = 0; i < 3; i++)
                {
                    auto _fetchTypeByte = *(*Tz::CmCustom::LS_45_type + i);
                    _itemNumArray[i] = Tz::CmCustom::GetCustomItemNum(_fetchTypeByte);

                    if (_isKH1Form)
                        break;
                }

                auto _fetchMaxLoop = *(*Tz::CmCustom::m_ListInfo + 0x04);
                auto _itemNumberCalc = 0x00;

                auto _processEntry = 0x00;

                if (_fetchCurPos && !_isKH1Form)
                    _fetchMaxLoop++;

                if (_fetchMaxLoop > 0x09)
                    _fetchMaxLoop = 0x09;

                for (int i = 0; i < 3; i++)
                {
                    auto _fetchTypeByte = *(*Tz::CmCustom::LS_45_type + i);

                    if (_processEntry >= _fetchMaxLoop)
                        break;

                    if (_itemNumArray[i])
                    {
                        for (int z = 0; z < _itemNumArray[i]; z++)
                        {
                            auto _fetchMessagePtr = _fetchBuffer + (0x220 * _processEntry) + 0x7858;
                            auto _fetchMessage = (_fetchBuffer + 0x20 * (z + 0x6E7)) + (0x20 * _itemNumberCalc);

                            if (_processEntry >= _fetchMaxLoop)
                                break;

                            if (z + _itemNumberCalc >= _fetchSelectTop)
                            {
                                if (z == 0x00 || _fetchTypeByte == 0x00)
                                {
                                    dk::Sprite::setNum(_fetchBuffer + 0x220 * _processEntry + 0x1060, _listSubSeq[_fetchTypeByte]);
                                    *reinterpret_cast<char**>(_fetchMessagePtr - 0x6600) = YS::MESSAGE::GetData(_fetchTypeByte ? _listSubMsg[_fetchTypeByte] : _listShortcutSubMsg[z]);
                                }

                                auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processEntry);
                                dk::Sprite::setNum(_fetchItemPtr, _listSelSeq[_fetchTypeByte]);

                                auto _fetchFontColor = 0x00;

                                if (_fetchTypeByte != 0x03 && !_isKH1Form)
                                    _fetchFontColor = Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * (_itemNumberCalc + z) + 0x08), 0x01, _fetchTypeByte == 0x01);

                                _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);
                                dk::Sprite::setNum(_fetchBuffer + 0x220 * _processEntry + 0x7660, _fetchFontColorSeq);
                                *reinterpret_cast<char**>(_fetchMessagePtr) = _fetchMessage;

                                _processEntry++;
                                _fetchMessagePtr += 0x220;
                            }
                        }
                    }

                    if (_isKH1Form)
                        break;

                    _itemNumberCalc += _itemNumArray[i];
                }
            }

            static void ChangeCustomInfo()
            {
                map<uint16_t, uint16_t> _limitShortcutMap =
                {
                    { 0x0239, 0x02BA },
                    { 0x023A, 0x02BD },
                    { 0x023B, 0x02C0 },
                    { 0x023C, 0x02AB }
                };

                auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);
                auto _fetchMenuSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                auto _isKH1Form = Tz::CmTop::GetCurPos(25) == 0x01 && Tz::CmCustom::CheckKH1Form();

                auto _fetchCommand = 0x00;

                switch (_fetchCustomType)
                {
                case 4:
                    Tz::CmCustom::ChageAbility(_fetchMenuSelectPos);
                    break;
                case 3:
                    Tz::CmCustom::ChangePartyBehavior(_fetchMenuSelectPos);
                    break;
                case 2:
                {
                    if (Tz::CmTop::GetSelectPos(25))
                        _fetchSelectPos--;

                    auto _fetchReplenishFlag = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0A) != 0x00;
                    auto _fetchBehaviorFlag = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0B) == 0x02;
                    auto _fetchOffset = _fetchReplenishFlag;

                    if (_fetchBehaviorFlag && Tz::CmTop::GetSelectPos(25))
                        _fetchOffset += 2;

                    if (_fetchOffset != _fetchMenuSelectPos);
                    Tz::CmCustom::ChangeAutoReplenishment();
                } break;
                case 1:
                {
                    auto _fetchMpFlag = Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0A != 0x00;

                    if (_fetchMpFlag != _fetchMenuSelectPos);
                    Tz::CmCustom::ChageMpDrive();
                } break;
                default:
                {
                    if (_fetchMenuSelectPos && !_isKH1Form)
                    {
                        auto _fetchInfo = *Tz::CmTop::m_ItemInfo + 0x04;
                        _fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 0x01)));
                    }

                    else if (_fetchMenuSelectPos)
                    {
                        auto _fetchInfo = *Tz::CmTop::m_ItemInfo + 0x04;

                        _fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 0x01)));

                        if (_fetchCommand == 0x0000)
                            _fetchCommand = _limitShortcutMap[*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 0x01))];
                    }

                    *reinterpret_cast<uint16_t*>(AREA::SaveData + (_fetchSelectPos * 0x02) + (_isKH1Form ? 0x371C : 0x36F8)) = _fetchCommand;
                } break;
                }

                memcpy(Tz::CmCustom::LS_KH1F_Shortcuts, AREA::SaveData + 0x371C, 0x08);

                Tz::CmCustom::GetListInfo(Tz::CmTop::GetSelectPos(25));
            }

            static void GetListInfo(int num)
            {
                vector<short> _magicItemNum = { 0x0015, 0x0016, 0x0017, 0x0018, 0x0057, 0x0058 };

                auto _fetchSheetNum = num >= 0x01 && Tz::CmCustom::CheckKH1Form() ? num - 1 : num;
                auto _fetchCurrentType = num == 0x00 ? Tz::CmCustom::s_PlayerType : (num == 0x01 && Tz::CmCustom::CheckKH1Form() ? Tz::CmCustom::s_PlayerType - 0x04 : Tz::CmCustom::s_FriendType);

                auto _isKH1Form = num == 0x01 && Tz::CmCustom::CheckKH1Form();

                memset(*Tz::CmCustom::m_ListInfo, 0x00, 0x98);

                auto _fetchPartySheet = Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, _fetchSheetNum);
                auto _fetchPartyInventory = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_fetchPartySheet + 0x250));

                auto _processMagic = 0x00;
                auto _processEntry = 0x00;

                if (_fetchPartyInventory < moduleInfo.moduleStart || _fetchPartyInventory > moduleInfo.moduleEnd)
                    return;

                for (int i = 0; i < 0x03; i++)
                {
                    if (_fetchCurrentType[i])
                    {
                        switch (_fetchCurrentType[i])
                        {
                        case 1:
                            for (int i = 0; i < 0x06; i++)
                            {
                                auto _fetchMagicId = _magicItemNum[i];

                                if (YS::ITEM::GetNumBackyard(_fetchMagicId))
                                {
                                    *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = _fetchMagicId;
                                    *(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = YS::MAGIC::GetMpDriveStatus(i);

                                    _processMagic++;
                                    _processEntry++;
                                }
                            }

                            **Tz::CmCustom::m_ListInfo = static_cast<char>(_processMagic);
                            break;
                        case 2:
                        {
                            unordered_set<uint32_t> _itemSet;
                            auto _itemCount = *reinterpret_cast<char*>(_fetchPartyInventory + 0x12);

                            for (int i = 0; i < _itemCount; i++)
                            {
                                auto _currItem = *reinterpret_cast<char*>(_fetchPartyInventory + 0x34 + 0x02 * i);

                                if (!_currItem)
                                    continue;

                                auto _autoReloadItem = *reinterpret_cast<char*>(_fetchPartyInventory + 0x44 + 0x02 * i);
                                _itemSet.insert((_autoReloadItem == _currItem) ? _currItem | 0x80000000 : _currItem);
                            }

                            for (int i = 0; i < _itemSet.size(); i++)
                            {
                                auto _fetchElement = next(_itemSet.begin(), i);

                                *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = *_fetchElement;
                                *(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = *_fetchElement & 0x80000000 ? 0x01 : 0x00;

                                if (_fetchCurrentType == Tz::CmCustom::s_FriendType)
                                {
                                    auto _fetchPriority = reinterpret_cast<char*>(_fetchPartyInventory + kn::FriendSaveRam::item2index((*_fetchElement & 0x0FFFFFFF)) + 0x0F5);
                                    *(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0B) = *_fetchPriority;
                                }

                                _processEntry++;
                            }

                            *(*Tz::CmCustom::m_ListInfo + 0x01) = _itemSet.size();
                        } break;
                        case 3:
                            *(*Tz::CmCustom::m_ListInfo + 0x03) = *reinterpret_cast<char*>(_fetchPartyInventory + 0x0F4);
                            break;
                        case 4:
                        {
                            vector<uint16_t> _abilityTable;

                            for (int i = 0; i < 0x50; i++)
                            {
                                auto _fetchAbility = *reinterpret_cast<uint16_t*>(_fetchPartyInventory + 0x54 + 0x02 * i);
                                auto _fetchAbilityAlone = _fetchAbility & 0x7FFF;

                                if (_fetchAbilityAlone == 0x00)
                                    continue;

                                auto _fetchItemType = *(YS::ITEM_TABLE::Get(_fetchAbilityAlone) + 0x06);

                                if (_fetchItemType == 0x02 && _fetchAbility & 0x8000)
                                    _abilityTable.push_back(_fetchAbilityAlone);
                            }

                            sort(_abilityTable.begin(), _abilityTable.end());

                            for (int i = 0; i < _abilityTable.size(); i++)
                            {
                                auto _fetchPriority = reinterpret_cast<char*>(_fetchPartyInventory + kn::FriendSaveRam::item2index(_abilityTable[i]) + 0x0F5);

                                *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = _abilityTable[i];

                                *(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = 1;
                                *(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0B) = *_fetchPriority;

                                _processEntry++;
                            }

                            *(*Tz::CmCustom::m_ListInfo + 0x02) = _abilityTable.size();
                        } break;
                        default:
                            continue;
                        }
                    }

                    else
                    {
                        for (int i = 0; i < 0x04; i++)
                        {
                            auto _fetchShortcut = *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x36F8 + 0x02 * i);
                            auto _fetchLimitShortcut = *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x371C + 0x02 * i);

                            if (_fetchShortcut && !_isKH1Form)
                            {
                                auto _fetchItem = YS::ITEM_TABLE::Each(nullptr);

                                while (_fetchItem)
                                {
                                    auto _fetchType = *(_fetchItem + 0x02);
                                    auto _fetchId = *reinterpret_cast<uint16_t*>(_fetchItem);
                                    auto _fetchCommand = YS::ITEM::GetCommand(_fetchId);

                                    if (((_fetchType == 0x12 || _fetchType == 0x15 || _fetchType == 0x00) && _fetchCommand == _fetchShortcut))
                                    {
                                        *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = _fetchId;

                                        if (_fetchType == 0x12)
                                        {
                                            auto _fetchMagicIdx = find(_magicItemNum.begin(), _magicItemNum.end(), *_fetchItem);
                                            auto _fetchDistance = distance(_magicItemNum.begin(), _fetchMagicIdx);

                                            *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = YS::MAGIC::GetMpDriveStatus(_fetchDistance);
                                        }

                                        break;
                                    }

                                    _fetchItem = YS::ITEM_TABLE::Each(_fetchItem);
                                }
                            }

                            else if (_fetchLimitShortcut && _isKH1Form)
                            {
                                auto _fetchItem = YS::ITEM_TABLE::Each(nullptr);
                                auto _fetchCommandPtr = reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08);

                                while (_fetchItem)
                                {
                                    auto _fetchType = *(_fetchItem + 0x02);
                                    auto _fetchId = *reinterpret_cast<uint16_t*>(_fetchItem);
                                    auto _fetchCommand = YS::ITEM::GetCommand(_fetchId);

                                    if (_fetchType == 0x00 && _fetchCommand == _fetchLimitShortcut)
                                    {
                                        *_fetchCommandPtr = _fetchId;
                                        break;
                                    }

                                    _fetchItem = YS::ITEM_TABLE::Each(_fetchItem);
                                }

                                if (*_fetchCommandPtr == 0x00)
                                    *_fetchCommandPtr = _fetchLimitShortcut;
                            }

                            else
                                *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = 0x00;

                            _processEntry++;
                        }
                    }

                    if (_isKH1Form)
                        break;
                }

                *(*Tz::CmCustom::m_ListInfo + 0x04) = _processEntry;
            }

            static void MakeListInfo2ItemMess()
            {
                int _itemNumArray[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                auto _fetchMenuMode = Tz::MenuBase::GetMode();
                auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());

                auto _getCurPos = Tz::CmTop::GetCurPos(0x19);

                if (_fetchMenuMode == 25)
                    _getCurPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                auto _isKH1Form = _getCurPos == 0x01 && Tz::CmCustom::CheckKH1Form();
                *Tz::CmCustom::LS_62_type = _getCurPos == 0x00 ? Tz::CmCustom::s_PlayerType : (_getCurPos == 0x01 && Tz::CmCustom::CheckKH1Form() ? Tz::CmCustom::s_PlayerType - 0x04 : Tz::CmCustom::s_FriendType);

                for (int i = 0; i < 3; i++)
                {
                    auto _fetchTypeByte = *(*Tz::CmCustom::LS_62_type + i);
                    _itemNumArray[i] = Tz::CmCustom::GetCustomItemNum(_fetchTypeByte);

                    if (*Tz::CmCustom::LS_62_type == Tz::CmCustom::s_PlayerType - 0x04)
                        break;
                }

                memset(_fetchBuffer + 0xDCE0, 0x00, 0x0360);

                auto _calcItemOffset = 0;
                auto _didProcessAI = false;

                for (int i = 0; i < 3; i++)
                {
                    auto _fetchCurrType = *(*Tz::CmCustom::LS_62_type + i);

                    for (int z = 0; z < _itemNumArray[i]; z++)
                    {
                        auto _isSpecial = false;
                        auto _processCommand = false;
                        char* _itemInfoStart = nullptr;
                        char* _fetchMainItem = nullptr;

                        int _fetchMessage = UINT32_MAX;
                        char* _messageAddr = nullptr;

                        uint16_t _fetchButtonIcon = UINT16_MAX;

                        if (_fetchCurrType)
                            _processCommand = _fetchCurrType == 0x01;

                        else
                        {
                            _processCommand = true;

                            if (_fetchMenuMode == 25)
                                _fetchButtonIcon = Tz::CmCustom::s_ButtonIcon[_calcItemOffset + z];
                        }

                        auto _fetchListInfo = *Tz::CmCustom::m_ListInfo;

                        if (_fetchCurrType == 0x03)
                        {
                            _messageAddr = _fetchBuffer + (_calcItemOffset + z + 0x06E7) * 0x20;
                            _fetchMessage = Tz::CmCustom::LS_57_s_MessTbl[*(*Tz::CmCustom::m_ListInfo + 0x03)];
                            _didProcessAI = true;
                        }

                        else
                        {
                            auto _fetchEntry = *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x08);
                            auto _fetchItem = YS::ITEM_TABLE::Get(_fetchEntry);
                            auto _fetchItemType = _fetchItem ? *(_fetchItem + 0x02) : -1;

                            if (_fetchEntry && (_fetchItemType == 0x12))
                            {
                                _isSpecial = *(*Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x0A) == 0x00;
                                _itemInfoStart = *Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI);
                            }

                            else
                            {
                                _isSpecial = *(_fetchListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x0A) == 0x01;
                                _itemInfoStart = _fetchListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI);
                            }

                            auto _fetchItemId = *reinterpret_cast<uint16_t*>(_itemInfoStart + 0x08);

                            _messageAddr = _fetchBuffer + (_calcItemOffset + z + 0x06E7) * 0x20;
                            _fetchMessage = 0x8448;

                            _fetchMainItem = _fetchItemId ? YS::ITEM_TABLE::Get(_fetchItemId) : nullptr;

                            if (_fetchButtonIcon != UINT16_MAX)
                                Tz::CmComm::FontIcon(_fetchButtonIcon, _messageAddr, true);

                            else if (_fetchMainItem)
                                Tz::CmComm::ItemIcon(*(_fetchMainItem + 2), _messageAddr, _isSpecial, true);

                            if (_fetchItemId)
                            {
                                if (_fetchMainItem)
                                {
                                    if (_processCommand)
                                    {
                                        auto _fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchMainItem));
                                        _fetchMessage = *reinterpret_cast<uint16_t*>(YS::COMMAND_ELEM::Get(_fetchCommand) + 0x08);
                                    }

                                    else
                                        _fetchMessage = *reinterpret_cast<uint16_t*>(_fetchMainItem + 0x08);
                                }

                                else
                                {
                                    _fetchMessage = *reinterpret_cast<uint16_t*>(YS::COMMAND_ELEM::Get(_fetchItemId) + 0x08);

                                    if (_fetchButtonIcon == UINT16_MAX)
                                    {
                                        Tz::CmComm::FontIcon(0xCF, _messageAddr, true);
                                        _messageAddr += 0x02;
                                    }
                                }
                            }

                            if (_fetchButtonIcon != UINT16_MAX || _fetchMainItem)
                                _messageAddr += 0x02;
                        }

                        auto _fetchData = YS::MESSAGE::GetData(_fetchMessage);
                        memcpy(_messageAddr, _fetchData, YS::MESSAGE::GetSize(_fetchData));

                        if (_getCurPos && !_isKH1Form)
                        {
                            if (_fetchCurrType == 0x02 || _fetchCurrType == 0x04)
                            {
                                auto _fetchFriend = Tz::CmCustom::GetFriendInfo(_fetchCurrType, *(_fetchListInfo + 0x08 * (_calcItemOffset + z) + 0x03));

                                if (_fetchFriend)
                                    Tz::CmComm::FontIcon(_fetchFriend[0x04], reinterpret_cast<char*>(_fetchBuffer) + 0x04 * (_calcItemOffset + z) + 0xDFE0, true);
                            }
                        }
                    }

                    _calcItemOffset += _itemNumArray[i];
                }
            }

            static void SetupTop()
            {
                auto _fetchBuffer = Tz::CmTop::GetListBuffer();
                Tz::SelHist::Create(0, -1);

                auto _fetchPriority = *Tz::CmCustom::m_pri;
                auto _processIdx = 0x00;
                *Tz::CmCustom::m_pri = 30017;

                for (int i = 0; i < 0x04; i++)
                {
                    if (Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, i))
                    {
                        auto _fetchPlateZero = Tz::CmTop::GetTopPlateSeqTbl(0);
                        auto _fetchPlateFirst = Tz::CmTop::GetTopPlateSeqTbl(1);

                        Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlateZero), reinterpret_cast<uint16_t*>(_fetchPlateFirst));
                        Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx, 0, -1);

                        auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx);

                        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
                        *reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = Tz::PartyInfo::GetName(*Tz::CmCustom::m_PartyInfo, i);
                        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

                        if (i == 0x00)
                        {
                            if (Tz::CmCustom::CheckKH1Form())
                            {
                                auto _fetchPlate2Zero = Tz::CmTop::GetTopPlateSeqTbl(0) + 0x10;
                                auto _fetchPlate2First = Tz::CmTop::GetTopPlateSeqTbl(1) + 0x10;

                                Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx + 1, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlate2Zero), reinterpret_cast<uint16_t*>(_fetchPlate2First));
                                Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx + 1, 0, -1);

                                auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx + 1);

                                *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
                                *reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = YS::MESSAGE::GetData(0x4E80);
                                *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

                                _processIdx++;
                            }
                        }

                        _processIdx++;
                    }
                }

                auto _fetchCurrPos = Tz::CmTop::GetCurPos(25);
                Tz::Select::SetCurrent(*Tz::CmTop::m_MenuPtr, _fetchCurrPos, true, 0x00, 0x02);
                Tz::MenuBase::SetSMode(0x00);
                *Tz::CmCustom::m_pri = _fetchPriority;
                Tz::CmCustom::CreateTopList();
                Tz::CmCustom::UpdateTopList();
            }

            static void UpdateTopList()
            {
                int _itemNumArray[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                auto _fetchBuffer = Tz::CmTop::GetListBuffer();
                auto _fetchCurrent = **Tz::CmTop::m_MenuPtr;

                Tz::CmCustom::GetListInfo(_fetchCurrent);
                Tz::CmCustom::MakeListInfo2ItemMess();

                auto _fetchSheetNum = _fetchCurrent > 0x01 && Tz::CmCustom::CheckKH1Form() ? _fetchCurrent - 1 : _fetchCurrent;
                *Tz::CmCustom::LS_103_type = _fetchCurrent == 0x00 ? Tz::CmCustom::s_PlayerType : (_fetchCurrent == 0x01 && Tz::CmCustom::CheckKH1Form() ? Tz::CmCustom::s_PlayerType - 0x04 : Tz::CmCustom::s_FriendType);

                auto _fetchFont = Tz::CmTop::GetFontColorSeqNum(0);

                auto _firstBuffer = reinterpret_cast<char*>(_fetchBuffer) + 0x1258;
                auto _secondBuffer = reinterpret_cast<char*>(_fetchBuffer) + 0x7660;

                for (int i = 0; i < 18; i++)
                {
                    dk::Sprite::setNum(_secondBuffer - 0x3300, *Tz::CmTop::m_DummySeq);
                    dk::Sprite::setNum(_secondBuffer, _fetchFont);

                    *_firstBuffer = 0;
                    *reinterpret_cast<char**>(_firstBuffer + 0x6600) = nullptr;

                    _secondBuffer += 0x220;
                    _firstBuffer += 0x220;
                }

                auto _fetchListInfo = *Tz::CmCustom::m_ListInfo;
                auto _calcItemOffset = 0x00;

                for (int i = 0; i < 3; i++)
                {
                    auto _fetchTypeByte = *(*Tz::CmCustom::LS_103_type + i);
                    _itemNumArray[i] = Tz::CmCustom::GetCustomItemNum(_fetchTypeByte);

                    if (*Tz::CmCustom::LS_103_type == Tz::CmCustom::s_PlayerType - 0x04)
                        break;
                }

                for (int i = 0; i < 0x03; i++)
                {
                    auto _itemIndex = *(*Tz::CmCustom::LS_103_type + i);
                    auto _fetchSeq = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint64_t>(Tz::CmCustom::LS_103_s_Seq) + 0x02 * _itemIndex);

                    if (_itemNumArray[i])
                    {
                        for (int z = 0; z < _itemNumArray[i]; z++)
                        {
                            auto _fetchBufferIndex = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * _calcItemOffset + 0x1258 + 0x220 * z;
                            auto _currOffset = _calcItemOffset - i;

                            auto _calcSeq = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * (_calcItemOffset + z) + 0x4360;
                            dk::Sprite::setNum(_calcSeq, _fetchSeq);

                            auto _fetchFontColor = 0x00;

                            if (_itemIndex != 0x03 && *Tz::CmCustom::LS_103_type != Tz::CmCustom::s_PlayerType - 0x04)
                                _fetchFontColor = Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z) + 0x08), 0x01, _itemIndex == 0x01);

                            auto _fetchFontColorSqd = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);
                            auto _calcFontSeq = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * (_calcItemOffset + z) + 0x7660;

                            dk::Sprite::setNum(_calcFontSeq, _fetchFontColorSqd);
                            *reinterpret_cast<char**>(_fetchBufferIndex + 0x6600) = reinterpret_cast<char*>(_fetchBuffer) + 32 * (_calcItemOffset + z + 0x6E7);

                            if (*Tz::CmCustom::LS_103_type == Tz::CmCustom::s_FriendType && _itemIndex != 0x03)
                                *reinterpret_cast<char**>(_fetchBufferIndex) = reinterpret_cast<char*>(_fetchBuffer) + 0x04 * (_calcItemOffset + z) + 0xDFE0;
                        }

                        _calcItemOffset = _calcItemOffset + _itemNumArray[i];
                    }
                }
            }

            static void UpdateHelpMess()
            {
                map<uint16_t, uint16_t> _limitShortcutMap =
                {
                    { 0x02BA, 0x0239 },
                    { 0x02BD, 0x023A },
                    { 0x02C0, 0x023B },
                    { 0x02AB, 0x023C }
                };

                short _magicHelpText[] = { 0x9DFB, 0x9DFA };
                short _itemHelpText[] = { 0x9DFF, 0x9DFE };
                short _itemFreqHelpText[] = { 0x9E5E, 0x9E5F };

                auto _fetchMenuMode = Tz::MenuBase::GetMode();
                auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                auto _fetchSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                auto _isKH1Form = ((_fetchMenuMode == 25 && _fetchSelectPos == 0x01) || (_fetchMenuMode != 25 && Tz::CmTop::GetCurPos(25) == 0x01)) && Tz::CmCustom::CheckKH1Form();

                uint16_t _messageId = UINT16_MAX;
                uint16_t _sideMessageId = UINT16_MAX;

                switch (_fetchMenuMode)
                {
                case 25:
                {
                    if (_fetchSelectPos == 0x00)
                        _messageId = 0x3B89;

                    else
                    {
                        if (_isKH1Form)
                            _messageId = 0x575C;

                        else
                            _messageId = 0x3B8A;
                    }
                } break;

                case 26:
                case 27:
                case 28:
                {
                    auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);

                    if (_fetchCustomType == 0x03)
                        _messageId = *reinterpret_cast<uint16_t*>(Tz::CmCustom::GetFriendInfo(0x03, *(*Tz::CmCustom::m_ListInfo + 3)) + 0x02);

                    else
                    {
                        auto _fetchMainPos = Tz::CmTop::GetCurPos(25);

                        if (_fetchMainPos && !_isKH1Form)
                            _fetchSelectPos -= 1;

                        auto _fetchItemId = *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x08);

                        if (_fetchItemId)
                        {
                            auto _fetchItemEntry = YS::ITEM_TABLE::Get(_fetchItemId);

                            if (_fetchItemEntry)
                                _messageId = *reinterpret_cast<uint16_t*>(_fetchItemEntry + 0x0A);

                            else if (_isKH1Form)
                            {
                                auto _fetchCommandItem = YS::ITEM_TABLE::Get(_limitShortcutMap[_fetchItemId]);
                                _messageId = *reinterpret_cast<uint16_t*>(_fetchCommandItem + 0x0A);
                            }

                            else
                                _messageId = *reinterpret_cast<uint16_t*>(YS::COMMAND_ELEM::Get(_fetchItemId) + 0x08);
                        }

                        if (_fetchMenuMode == 27 && !_isKH1Form)
                            _sideMessageId = *reinterpret_cast<uint16_t*>(Tz::CmCustom::GetFriendInfo(_fetchCustomType, *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0B)) + 0x02);
                    }
                } break;
                case 29:
                {
                    if (_fetchSelectPos > 0x00)
                    {
                        auto _itemInfo = reinterpret_cast<char*>(*Tz::CmTop::m_ItemInfo + 0x04);
                        auto _fetchItemId = *reinterpret_cast<uint16_t*>(_itemInfo + 0x06 * (_fetchSelectPos - 0x01));
                        auto _fetchItemEntry = YS::ITEM_TABLE::Get(_fetchItemId);

                        if (_fetchItemEntry)
                            _messageId = *reinterpret_cast<uint16_t*>(_fetchItemEntry + 0x0A);
                    }

                    else
                        _messageId = 0x1DF7;
                } break;

                case 30:
                    _messageId = _magicHelpText[_fetchSelectPos];
                    break;

                case 31:
                {
                    auto _fetchMainPos = Tz::CmTop::GetCurPos(25);

                    _messageId = _itemHelpText[_fetchSelectPos % 0x02];

                    if (_fetchMainPos && !_isKH1Form)
                        _sideMessageId = _itemFreqHelpText[_fetchSelectPos / 0x02];
                } break;

                default:
                    _messageId = *reinterpret_cast<uint16_t*>(Tz::CmCustom::GetFriendInfo(_fetchMenuMode == 32 ? 0x03 : 0x04, Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr)) + 0x02);
                    break;
                }

                if (_messageId != UINT16_MAX)
                {
                    auto _fetchData = YS::MESSAGE::GetData(_messageId);
                    memcpy(_fetchBuffer + 0xE0A0, _fetchData, YS::MESSAGE::GetSize(_fetchData) + 0x01);

                    if (_sideMessageId != UINT16_MAX)
                    {
                        auto _fetchSideData = YS::MESSAGE::GetData(_sideMessageId);
                        memcpy(_fetchBuffer + 0xE0A0 + YS::MESSAGE::GetSize(_fetchData), _fetchSideData, YS::MESSAGE::GetSize(_fetchSideData) + 0x01);
                    }
                }

                Tz::CMenuHelp::Create(_messageId != UINT16_MAX ? _fetchBuffer + 0xE0A0 : nullptr, true);
            }

            static bool isTakeOff()
            {
                auto _fetchMode = Tz::MenuBase::GetMode();
                auto _fetchCurrentPos = Tz::CmTop::GetSelectPos(26);
                auto _fetchTopSelectPos = Tz::CmTop::GetSelectPos(25);

                auto _isKH1Form = _fetchTopSelectPos == 0x01 && Tz::CmCustom::CheckKH1Form();

                if (_fetchMode == 26 || _fetchMode == 27)
                    _fetchCurrentPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

                if (_fetchTopSelectPos)
                    _fetchCurrentPos--;

                auto _fetchItem = *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x08);
                auto _fetchFlagCheck = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) != 0x00;

                if (_fetchMode != 26 && _fetchMode != 27)
                    _fetchFlagCheck = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr) % 0x02 == 0x00;

                if (!_fetchFlagCheck)
                    return false;

                auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, _fetchTopSelectPos && Tz::CmCustom::CheckKH1Form() ? _fetchTopSelectPos - 1 : _fetchTopSelectPos) + 0x250);
                auto _convertSheet = reinterpret_cast<char*>(PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet));

                auto _fetchPartyCount = *(_convertSheet + 0x12);

                if (_fetchPartyCount == 0x00)
                    return false;

                for (int i = 0; i < _fetchPartyCount; i++)
                {
                    auto _fetchCurrentItemPtr = _convertSheet + 0x44 + 0x02 * i;

                    auto _fetchCurrItem = *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr - 0x10);
                    auto _fetchCurrReplenish = *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr);

                    auto _continueCheck = _fetchCurrItem == _fetchItem || _fetchCurrReplenish != _fetchItem;

                    if (!_continueCheck)
                        return true;
                }

                return false;
            }

            static bool CheckKH1Form()
            {
                return YS::ITEM::GetNumBackyard(0x0233) && AREA::Current->World != 0x0A && AREA::Current->World != 0x0B && CAN_ALTER_KH1F;
            }

            static void CtrlCustomList()
            {
                uint16_t IndiCustomSelSeq[] = { 0x00CA, 0x00C1, 0x00B8, 0x00A6, 0x009D };
                uint16_t IndiCustomTitleSeq[] = { 0x00CE, 0x00C5, 0x00BC, 0x00AA, 0x00A1, 0x0000 };
                uint16_t IndiCustomBaseSeq[] = { 0x00C6, 0x00C8, 0x00C7, 0x00BD, 0x00BF, 0x00BE, 0x00B4, 0x00B6, 0x00B5, 0x00A2, 0x00A4, 0x00A3, 0x0099, 0x009B, 0x009A, 0x0000 };

                auto _fetchSelect = Tz::Select::select(*Tz::CmTop::m_MenuPtr);
                auto _fetchSMode = Tz::MenuBase::GetSMode();

                if (_fetchSMode == 0x02)
                {
                    auto _fetchNextMode = Tz::MenuBase::GetNextMode();
                    auto _fetchIsExist = false;

                    if (_fetchNextMode >= 25 && _fetchNextMode <= 27)
                        _fetchIsExist = Tz::SelHist::isExist(1);

                    if (_fetchNextMode != 26 && _fetchNextMode != 27)
                        _fetchIsExist = (_fetchIsExist || Tz::CmTop::isExistPadHelp()) ? true : false;

                    if (Tz::CmTop::isExistListAll()
                        || *Tz::CmTop::m_SclBar && dk::Obj2D::isExist(*reinterpret_cast<char**>(*Tz::CmTop::m_SclBar))
                        || _fetchIsExist
                        || Tz::CmTop::isExistNew())
                    {
                        if (_fetchSelect != UINT32_MAX || Tz::Select::isCursorMove(*Tz::CmTop::m_MenuPtr))
                        {
                            Tz::MenuBase::SavePad(nullptr);
                            Tz::CmTop::LeaveListAll();

                            if (Tz::Select::isExist(*Tz::CmTop::m_MenuPtr))
                                Tz::Select::Leave(*Tz::CmTop::m_MenuPtr);

                            if (dk::Obj2D::isExist(*reinterpret_cast<char**>(*Tz::CmTop::m_SclBar)))
                                Tz::ScrollBar::Leave(*Tz::CmTop::m_SclBar);

                            if (_fetchNextMode != 26 && _fetchNextMode != 27)
                                Tz::CmTop::LeavePadHelp();
                        }
                    }

                    else
                    {
                        Tz::MenuBase::NextMode2Mode();
                        *reinterpret_cast<uint32_t*>(Tz::CmTop::m_MenuPtr + 0x40) = 115;

                        if (_fetchNextMode == 25)
                        {
                            Tz::SelHist::Leave(1);
                            SetupTop();
                        }

                        else if (_fetchNextMode == 26 || _fetchNextMode == 27)
                        {
                            Tz::CmTop::SaveCurPos(25, *s_ChgSelCharaNext, 0);

                            if (*s_ChgSelCharaNext == 1 && CheckKH1Form())
                                Tz::SelHist::SetMsg(1, YS::MESSAGE::GetData(0x4E80));

                            else
                                Tz::SelHist::SetMsg(1, Tz::PartyInfo::GetName(*m_PartyInfo, (*s_ChgSelCharaNext > 0x01 && CheckKH1Form()) ? *s_ChgSelCharaNext - 1 : *s_ChgSelCharaNext));

                            *s_ChgSelCharaNext = UINT32_MAX;

                            auto _fetchCurrPos = Tz::CmTop::GetCurPos(25);
                            GetListInfo(_fetchCurrPos);
                            MakeListInfo2ItemMess();
                            SetupCustomList();
                        }

                        else
                            SetupCustom();
                    }

                    return;
                }
            
                if (_fetchSelect != UINT32_MAX || Tz::Select::isCursorMove(*Tz::CmTop::m_MenuPtr))
                {
                    auto _fetchListBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                    auto _fetchMode = Tz::MenuBase::GetMode();

                    auto _fetchListEquity = 228;

                    if (_fetchMode != 26 && _fetchMode != 27)
                    {
                        auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                        _fetchListEquity = IndiCustomBaseSeq[0x03 * Tz::CmCustom::CurPos2CustomType(_fetchSelectPos)];
                    }

                    if (!_fetchListBuffer || !dk::Obj2D::isExist(_fetchListBuffer) || *reinterpret_cast<uint32_t*>(_fetchListBuffer + 0x1D4) != _fetchListEquity)
                    {
                        if (dk::Obj2D::isExist(*reinterpret_cast<char**>(*Tz::CmTop::m_SclBar)))
                            Tz::ScrollBar::Loop(*Tz::CmTop::m_SclBar);

                        Tz::SelHist::Loop(UINT32_MAX);
                        Tz::CmTop::LoopPadHelp();
                    }
                }

                if (!_fetchSMode)
                {
                    auto _fetchListBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                    auto _fetchMode = Tz::MenuBase::GetMode();

                    auto _fetchListEquity = 228;

                    if (_fetchMode != 26 && _fetchMode != 27)
                    {
                        auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                        _fetchListEquity = IndiCustomBaseSeq[0x03 * Tz::CmCustom::CurPos2CustomType(_fetchSelectPos)];
                    }

                    if (*reinterpret_cast<uint32_t*>(_fetchListBuffer + 0x1D4) != _fetchListEquity)
                    {
                        UpdateHelpMess();
                        Tz::MenuBase::IncSMode(1);
                    }
                }

                auto _fetchSelectMax = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x16);
                auto _fetchCursorMove = Tz::Select::isCursorMove(*Tz::CmTop::m_MenuPtr);
                auto _fetchPadSE = Tz::MenuBase::PadSE(_fetchSelect, _fetchCursorMove, true, SelCheck, true);

                if (!Tz::MenuSound::isBeep(_fetchPadSE) && _fetchSelect != UINT32_MAX)
                {
                    uint16_t _listSubMsg[] = { 0x847D, 0x8481, 0x8482, 0x8483, 0x8484, 0x0000 };
                    uint16_t _listShortcutSubMsg[] = { 0x847D, 0x847E, 0x847F, 0x8480 };

                    switch (_fetchSelect)
                    {
                        case -6:
                        {
                            auto _fetchSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);
                            auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);

                            switch (_fetchCustomType)
                            {
                            case 1:
                                ChageMpDrive();
                                break;
                            case 2:
                                ChangeAutoReplenishment();
                                break;
                            case 3:
                                ChangePartyBehavior(UINT32_MAX);
                                break;
                            case 4:
                                ChageAbility(UINT32_MAX);
                                break;
                            }

                            if (isTakeOff())
                            {
                                ShowErrorTakeOff();
                                Tz::MenuBase::SetSMode(0);
                                Tz::MenuBase::SetMode(28, 0);
                            }

                            MakeListInfo2ItemMess();
                            UpdateHelpMess();
                        } break;
                        case -2:
                        {
                            Tz::CMenuHelp::FadeOut();
                            Tz::CmTop::FadeOutPadHelp();
                            Tz::SelHist::FadeOut(1);

                            auto _fetchListBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                            auto _fetchMode = Tz::MenuBase::GetMode();

                            auto _fetchFadeSeq = 229;

                            if (_fetchMode >= 26 && _fetchMode <= 33)
                            {
                                if (_fetchMode != 26 && _fetchMode != 27)
                                {
                                    auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                                    _fetchFadeSeq = IndiCustomBaseSeq[0x03 * Tz::CmCustom::CurPos2CustomType(_fetchSelectPos)];
                                }

                                Tz::MenuUtil::SprtFadeOut(_fetchListBuffer, _fetchFadeSeq);

                                if (dk::Obj2D::isExist(*reinterpret_cast<char**>(*Tz::CmTop::m_SclBar)))
                                    Tz::ScrollBar::FadeOut(*Tz::CmTop::m_SclBar);
                            }

                            Tz::MenuBase::IncSMode(1);
                            Tz::MenuBase::SetNextMode(25);

                            auto _fetchCurrent = *reinterpret_cast<uint32_t*>(*Tz::CmTop::m_MenuPtr);
                            auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);

                            Tz::CmTop::SaveCurPos(26, _fetchCurrent, _fetchSelectTop);
                        } break;
                        case -4:
                        {
                            Tz::MenuBase::SetExit();

                            auto _fetchCurrent = *reinterpret_cast<uint32_t*>(*Tz::CmTop::m_MenuPtr);
                            auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);

                            Tz::CmTop::SaveCurPos(26, _fetchCurrent, _fetchSelectTop);
                        } break;
                        case -7:
                        {
                            auto _fetchSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);
                            auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);
                            auto _fetchMode = Tz::MenuBase::GetMode();

                            Tz::MenuBase::SetMode(37, 0);

                            if (_fetchCustomType == 2)
                                Tz::CmTutorial::Setup(0, _fetchMode);

                            else if (_fetchCustomType == 1)
                                Tz::CmTutorial::Setup(1, _fetchMode);
                        } break;
                        case -10:
                        case -11:
                        {
                            auto _fetchCurPos = Tz::CmTop::GetCurPos(25);
                            auto _fetchPartyMax = *reinterpret_cast<uint8_t*>(*m_PartyInfo) + CheckKH1Form();

                            if (_fetchSelect + 10)
                            {
                                auto _calculateParty = _fetchCurPos - 1;
                                *s_ChgSelCharaNext = _calculateParty < 0 ? _fetchPartyMax - 1 : _calculateParty;
                            }

                            else
                            {
                                auto _calculateParty = _fetchCurPos + 1;
                                *s_ChgSelCharaNext = _calculateParty >= _fetchPartyMax ? 0 : _calculateParty;
                            }

                            Tz::CMenuHelp::FadeOut();
                            Tz::CmTop::FadeOutPadHelp();
                            Tz::SelHist::FadeOut(1);

                            auto _fetchListBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                            auto _fetchMode = Tz::MenuBase::GetMode();

                            auto _fetchFadeSeq = 229;

                            if (_fetchMode >= 26 && _fetchMode <= 33)
                            {
                                if (_fetchMode != 26 && _fetchMode != 27)
                                {
                                    auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                                    _fetchFadeSeq = IndiCustomBaseSeq[0x03 * Tz::CmCustom::CurPos2CustomType(_fetchSelectPos)];
                                }

                                Tz::MenuUtil::SprtFadeOut(_fetchListBuffer, _fetchFadeSeq);

                                if (dk::Obj2D::isExist(*reinterpret_cast<char**>(*Tz::CmTop::m_SclBar)))
                                    Tz::ScrollBar::FadeOut(*Tz::CmTop::m_SclBar);
                            }

                            Tz::MenuBase::IncSMode(1);
                            Tz::MenuBase::SetNextMode(*s_ChgSelCharaNext ? 27 : 26);

                            Tz::CmTop::SaveCurPos(26, 0, 0);
                        }break;
                        default:
                        {
                            auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelect);

                            if (_fetchCustomType)
                                Tz::SelHist::SetMsgId(2, _listSubMsg[_fetchCustomType]);
                            else
                                Tz::SelHist::SetMsgId(2, _listShortcutSubMsg[_fetchSelect]);

                            Tz::CmTop::FadeOutPadHelp();
                            Tz::CMenuHelp::FadeOut();

                            auto _fetchListBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
                            auto _fetchMode = Tz::MenuBase::GetMode();

                            auto _fetchFadeSeq = 229;

                            if (_fetchMode >= 26 && _fetchMode <= 33)
                            {
                                if (_fetchMode != 26 && _fetchMode != 27)
                                {
                                    auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
                                    _fetchFadeSeq = IndiCustomBaseSeq[0x03 * Tz::CmCustom::CurPos2CustomType(_fetchSelectPos)];
                                }

                                Tz::MenuUtil::SprtFadeOut(_fetchListBuffer, _fetchFadeSeq);

                                if (dk::Obj2D::isExist(*reinterpret_cast<char**>(*Tz::CmTop::m_SclBar)))
                                    Tz::ScrollBar::FadeOut(*Tz::CmTop::m_SclBar);
                            }

                            Tz::MenuBase::IncSMode(1);
                            Tz::MenuBase::SetNextMode(29 + _fetchCustomType);

                            auto _fetchCurPos = Tz::CmTop::GetCurPos(25);

                            if (*reinterpret_cast<uint32_t*>(*Tz::CmTop::m_MenuPtr) != _fetchCurPos)
                                Tz::CmTop::SaveCurPos(26, 0, 0);

                            auto _fetchCurrent = *reinterpret_cast<uint32_t*>(*Tz::CmTop::m_MenuPtr);
                            auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);

                            Tz::CmTop::SaveCurPos(26, _fetchCurrent, _fetchSelectTop);
                        } break;
                    }
                }

                    Tz::MenuBase::ResetPad();
            }

        private:
            static bool _init()
            {
                RedirectRelativeFunction("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F, reinterpret_cast<uint64_t>(UpdateTopList), 0x2B6);

                RedirectFunction("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x8B\xF8\x8D\x48\xE6\x83\xF9\x01\x76\x0C", "xxxxxxxxxxx????xxxxxxxxxx", reinterpret_cast<uint64_t>(isTakeOff), 0xF8);
                RedirectFunction("\x40\x53\x57\x48\x83\xEC\x48\x48\x89\x6C\x24\x60\x4C\x89\x64\x24\x70\x4C\x89\x6C\x24\x40\x4C\x89", "xxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(ChangeAutoReplenishment), 0x028A);
                RedirectFunction("\x40\x53\x48\x83\xEC\x20\x8B\xD9\xB9\x19\x00\x00\x00\xE8", "xxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(ChangePartyBehavior), 0x75);
                RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x8B\xF9\xE8\x00\x00\x00\x00\xB9\x19\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxxx", reinterpret_cast<uint64_t>(ChageAbility), 0xFA);
                RedirectFunction("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\xB9\x1A\x00\x00\x00\xE8", "xxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(ChangeCustomInfo), 0x160);
                RedirectFunction("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xF9\xB9\x19\x00\x00\x00\xE8", "xxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(CurPos2CustomType), 0xBA);
                RedirectFunction("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", reinterpret_cast<uint64_t>(SetupCustom), 0x555);
                RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x60\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x58", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxx", reinterpret_cast<uint64_t>(UpdateCustomList), 0x415);
                RedirectFunction("\x40\x53\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x28\x48\x89\x7C\x24\x60\x4C\x89\x74\x24\x20\xE8", "xxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(UpdateHelpMess), 0x317);
                RedirectFunction("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x63\xD8\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x8B\xF0\x83\xFB\x19\x75\x0E", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxx????xxxxxxxxxxxxx", reinterpret_cast<uint64_t>(MakeListInfo2ItemMess), 0x3EA);
                RedirectFunction("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(SetupTop), 0x235);
                RedirectFunction("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(GetListInfo), 0x65C);
                RedirectFunction("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x63\xF8", "xxxxxxxxxxxxxxxxxx????x????xxx", reinterpret_cast<uint64_t>(CtrlCustomList), 0x595);

                return true;
            }

#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
#endif
        };
    }
}