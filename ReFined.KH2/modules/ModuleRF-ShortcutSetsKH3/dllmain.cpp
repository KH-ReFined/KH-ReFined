#include <map>
#include <cstdint>
#include <algorithm>
#include <unordered_set>

#include <Windows.h>
#include <Psapi.h>

using namespace std;
HMODULE MAIN_HANDLE;

struct MODULE_INFO
{
    char* moduleStart;
    char* moduleEnd;

    MODULE_INFO()
    {
        auto hModule = GetModuleHandle(NULL);
        moduleStart = reinterpret_cast<char*>(hModule);

        MODULEINFO _fetchInfo = {};
        GetModuleInformation(GetCurrentProcess(), hModule, &_fetchInfo, sizeof(_fetchInfo));

        moduleEnd = moduleStart + _fetchInfo.SizeOfImage;
    }
};

const MODULE_INFO moduleInfo;

void RedirectFunctionSLIM(const char* functionSymbol, uint64_t function)
{
    DWORD _oldProtect;
    auto _fetchFunction = reinterpret_cast<char*>(GetProcAddress(MAIN_HANDLE, functionSymbol));

    vector<uint8_t> _absoluteInstructionJMP =
    {
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    if (*_fetchFunction == (char)0xE9 || *_fetchFunction == (char)0xE8)
    {
        auto _fetchOffset = *reinterpret_cast<uint32_t*>(_fetchFunction + 0x01);
        _fetchFunction += _fetchOffset + 0x05;
    }

    VirtualProtect(_fetchFunction, 4096, PAGE_EXECUTE_READWRITE, &_oldProtect);

    memset(_fetchFunction, 0x90, _absoluteInstructionJMP.size());

    memcpy(_absoluteInstructionJMP.data() + 0x06, &function, 0x08);
    memcpy(_fetchFunction, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
}

bool SYNC_SHORTCUTS;
bool DEBOUNCE_SHORTCUT;

bool CAN_ALTER_KH1F;

int* m_pri;

char* s_SelSeq;
char* s_PlayerType;
char* s_FriendType;
short* s_ButtonIcon;

char** m_ListInfo;
char** m_PartyInfo;

char** LS_45_type;
char** LS_52_type;
char** LS_62_type;
char** LS_103_type;

char* LS_103_s_Seq;
int* LS_57_s_MessTbl;

char* LS_KH1F_Shortcuts;

char* DrawItemAILv;

void(*ChageMpDrive)();
void(*CreateTopList)();
void(*UpdateIndiCustomList)();
void(*SetIndiCustomDefaultPos)();

char* (*GetFriendInfo)(int type, int num);

class AREA
{
    public:

    struct SET
    {
        short Map;
        short Battle;
        short Event;
    };

    struct INFO
    {
        char World;
        char Room;
        char Entrance;
        char Padding;
        AREA::SET Set;
    };

    static inline AREA::INFO* Current;
    static inline char* SaveData;
    static inline bool* IsInMap;
};

class MENU
{
public:
    static inline bool* IsMenu;
};

class SOUND
{
    public:
    static inline void(*PlaySFX)(uint32_t soundID);
};

namespace dk
{
    class COMMAND_DRAW
    {
        public:
        static inline char** CommandMenu;
    };

    class Sprite
    {
        public:
        static inline void (*setNum)(char* Sprite, int num);
    };
}

namespace kn
{
    class FriendSaveRam
    {
        public:
        static inline int (*item2index)(int item_id);
    };
}

namespace PC
{
    class CONVERTER
    {
        public:
        static inline char* (*INTPTR_TO_POINTER)(uint32_t);
    };
}

namespace Tz
{
    class CmTop
    {
        public:
        static inline char** (*GetListBuffer)();
        
        static inline int    (*GetCurPos)(int num);
        static inline char*  (*GetScrBarSeqTbl)();
        static inline int    (*GetSelectPos)(int num);
        static inline int    (*GetFontColorSeqNum)(int num);
        static inline char*  (*GetTopPlateSeqTbl)(int num);
        static inline int    (*GetItemFontColor)(int id, int cnt, bool special);
        
        static inline int*   m_SeqUnit;
        static inline int*   m_ImgUnit;
        static inline int*   m_DummySeq;
        
        static inline char** m_Lay;
        static inline char** m_SclBar;
        static inline char** m_MenuPtr;
        static inline char** m_ItemInfo;
    };

    class CmComm
    {
        public:
        static inline void (*FontIcon)(char icon, char* buff, bool terminate);
        static inline void (*ItemIcon)(int type, char* buff, bool isSpecial, bool terminate);
    };

    class CMenuHelp
    {
        public:
        static inline void (*Create)(char* message, bool param_xy);
    };

    class ItemInfo
    {
        public:
        static inline void (*SortItemInfo)(char* itemInfo);
        static inline void (*AddKnowItemInfo)(char* itemInfo);
        static inline void (*MakeEmptyMsg)(char* itemInfo, int messageId);
        static inline int  (*MakeCommandMsg)(char* itemInfo, int pos, int max);
        static inline void (*SetItemInfo)(char* itemInfo, int type, int item_type);
    };

    class MenuBase
    {
        public:
        static inline int  (*GetMode)();
        static inline void (*SetSMode)(int mode);
    };

    class MenuUtil
    {
        public:
        static inline void  (*SetSprtParent)(char* target, char* parent);
        static inline char* (*CreateSprt)(char* spriteMessage, int priority, uint32_t s_unit, uint32_t i_unit, char* layout, int fin, uint32_t loop, int group, int offset16x9);
        static inline char* (*CreateMess)(char* spriteMessage, int priority, uint32_t s_unit, uint32_t i_unit, char* layout, int fin, uint32_t loop, int messageNo, int group, int offset16x9);
    };

    class PartyInfo
    {
        public:
        static inline char* (*GetName)(char* partyInfo, int num);
        static inline char* (*GetSheet)(char* partyInfo, int num);
    };

    class ScrollBar
    {
        public:
        static inline void (*Create)(char* scrollBar, int pri, char* sqd, char* img, char* seq, int max, int dips, char* parent);
        static inline void (*SetPos)(char* scrollBar, int pos);
        static inline void (*SetMax)(char* scrollBar, int max);
    };

    class Select
    {
        public:
        static inline int   (*GetSelectPos)(char* select);
        static inline void  (*SetPageMode)(char* select, int mode);
        static inline void  (*SetSelectMax)(char* select, int max);
        static inline int   (*SetSelectTop)(char* select, int pos);
        static inline char* (*GetItemPtr)(char* select, uint64_t pos);
        static inline void  (*SetExOffset)(char* select, int pos, int x, int y);
        static inline void  (*SetCurrent)(char* select, int pos, bool isSet, int num, int next);
        static inline void  (*SetItem)(char* select, int pos, uint32_t s_unit, uint32_t i_unit, char* layout, int message, uint16_t* seq_n, char* seq_a);
    };

    class SelHist
    {
        public:
        static inline void (*Create)(int num, int msg);
    };
}

namespace YI
{
    class IMAGE_FREEZE
    {
        public:
        static inline char* (*GetImage)(char* imageFreeze, int no);
    };

    class LAYOUT
    {
        public:
        static inline char* (*GetSequenceUnit)(char* layout, int no);
    };

    class SEQUENCE
    {
        public:
        static inline int(*GetParamCr)(char* SEQUENCE);
    };
}

namespace YS
{
    class COMMAND_ELEM
    {
        public:
        static inline char* (*Get)(uint64_t id);
    };

    class HARDPAD
    {
    public:
        static inline uint16_t* Input;
    };


    class ITEM
    {
        public:
        static inline int (*GetCommand)(uint64_t item);
        static inline uint64_t(*GetNumBackyard)(uint64_t item);
    };

    class ITEM_TABLE
    {
        public:
        static inline char* (*Each)(char* itemTable);
        static inline char* (*Get)(uint64_t itemId);
    };

    class MAGIC
    {
        public:
        static inline bool (*GetMpDriveStatus)(int index);
    };

    class MESSAGE
    {
        public:
        static inline char* (*GetData)(int id);
        static inline const size_t (*GetSize)(const char* message);
    };

    class TITLE
    {
        public:
        static inline bool* IsTitle;
    };
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

extern "C"
{
    bool CheckKH1Form()
    {
        return YS::ITEM::GetNumBackyard(0x0233) && AREA::Current->World != 0x0A && AREA::Current->World != 0x0B && CAN_ALTER_KH1F;
    }

    int GetCustomItemNum(int type)
    {
        auto _fetchListInfo = *m_ListInfo;

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

        return 0x04;
    }

    int CurPos2SheetNum(int pos)
    {
        if (pos < 0x04 || (pos == 0x04 && CheckKH1Form()))
            return 0x00;

        else
            return pos - (0x03 + CheckKH1Form());
    }

    char* CurPos2CurrType(int pos)
    {
        if (pos == 0x00)
            return s_PlayerType;

        else if (pos < 0x04 || (pos == 0x04 && CheckKH1Form()))
            return s_PlayerType - 0x04;

        else
            return s_FriendType;
    }

    void GetListInfo(int num)
    {
        vector<short> _magicItemNum = { 0x0015, 0x0016, 0x0017, 0x0018, 0x0057, 0x0058 };

        auto _fetchCurrentType = CurPos2CurrType(num);

        auto _isKH1Form = num == 0x04 && CheckKH1Form();
        auto _isShortcutSet = num != 0x00 && num < 0x04;

        memset(*m_ListInfo, 0x00, 0x98);

        auto _fetchPartySheet = Tz::PartyInfo::GetSheet(*m_PartyInfo, CurPos2SheetNum(num));
        auto _fetchPartyInventory = PC::CONVERTER::INTPTR_TO_POINTER(*reinterpret_cast<uint32_t*>(_fetchPartySheet + 0x250));

        auto _processMagic = 0x00;
        auto _processEntry = 0x00;

        for (int i = num == 0x00 ? 0x01 : 0x00; i < 0x03; i++)
        {
            if (_fetchCurrentType[i])
            {
                switch (_fetchCurrentType[i])
                {
                case 1:
                    for (int z = 0; z < 0x06; z++)
                    {
                        auto _fetchMagicId = _magicItemNum[z];

                        if (YS::ITEM::GetNumBackyard(_fetchMagicId))
                        {
                            *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x08) = _fetchMagicId;
                            *(*m_ListInfo + 0x08 * _processEntry + 0x0A) = YS::MAGIC::GetMpDriveStatus(z);

                            _processMagic++;
                            _processEntry++;
                        }
                    }

                    **m_ListInfo = static_cast<char>(_processMagic);
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

                        *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x08) = *_fetchElement;
                        *(*m_ListInfo + 0x08 * _processEntry + 0x0A) = *_fetchElement & 0x80000000 ? 0x01 : 0x00;

                        if (_fetchCurrentType == s_FriendType)
                        {
                            auto _fetchPriority = reinterpret_cast<char*>(_fetchPartyInventory + kn::FriendSaveRam::item2index((*_fetchElement & 0x0FFFFFFF)) + 0x0F5);
                            *(*m_ListInfo + 0x08 * _processEntry + 0x0B) = *_fetchPriority;
                        }

                        _processEntry++;
                    }

                    *(*m_ListInfo + 0x01) = _itemSet.size();
                } break;
                case 3:
                    *(*m_ListInfo + 0x03) = *reinterpret_cast<char*>(_fetchPartyInventory + 0x0F4);
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

                        *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x08) = _abilityTable[i];

                        *(*m_ListInfo + 0x08 * _processEntry + 0x0A) = 1;
                        *(*m_ListInfo + 0x08 * _processEntry + 0x0B) = *_fetchPriority;

                        _processEntry++;
                    }

                    *(*m_ListInfo + 0x02) = _abilityTable.size();
                } break;
                default:
                    continue;
                }
            }

            else
            {
                for (int i = 0; i < 0x04; i++)
                {
                    auto _shortcutPtr = _isKH1Form ? AREA::SaveData + 0x371C : AREA::SaveData + 0x10108 + (num - 0x01) * 0x08;
                    auto _fetchShortcut = *reinterpret_cast<uint16_t*>(_shortcutPtr + 0x02 * i);

                    if (_fetchShortcut)
                    {
                        if (!_isKH1Form)
                        {
                            auto _fetchItem = YS::ITEM_TABLE::Each(nullptr);

                            while (_fetchItem)
                            {
                                auto _fetchType = *(_fetchItem + 0x02);
                                auto _fetchId = *reinterpret_cast<uint16_t*>(_fetchItem);
                                auto _fetchCommand = YS::ITEM::GetCommand(_fetchId);

                                if (((_fetchType == 0x12 || _fetchType == 0x15 || _fetchType == 0x00) && _fetchCommand == _fetchShortcut))
                                {
                                    *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x08) = _fetchId;

                                    if (_fetchType == 0x12)
                                    {
                                        auto _fetchMagicIdx = find(_magicItemNum.begin(), _magicItemNum.end(), *_fetchItem);
                                        auto _fetchDistance = distance(_magicItemNum.begin(), _fetchMagicIdx);

                                        *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x0A) = YS::MAGIC::GetMpDriveStatus(_fetchDistance);
                                    }

                                    break;
                                }

                                _fetchItem = YS::ITEM_TABLE::Each(_fetchItem);
                            }
                        }

                        else if (_isKH1Form)
                        {
                            auto _fetchItem = YS::ITEM_TABLE::Each(nullptr);
                            auto _fetchCommandPtr = reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x08);

                            while (_fetchItem)
                            {
                                auto _fetchType = *(_fetchItem + 0x02);
                                auto _fetchId = *reinterpret_cast<uint16_t*>(_fetchItem);
                                auto _fetchCommand = YS::ITEM::GetCommand(_fetchId);

                                if (_fetchType == 0x00 && _fetchCommand == _fetchShortcut)
                                {
                                    *_fetchCommandPtr = _fetchId;
                                    break;
                                }

                                _fetchItem = YS::ITEM_TABLE::Each(_fetchItem);
                            }

                            if (*_fetchCommandPtr == 0x00)
                                *_fetchCommandPtr = _fetchShortcut;
                        }
                    }

                    else
                        *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _processEntry + 0x08) = 0x00;

                    _processEntry++;
                }
            }

            if (_isKH1Form || _isShortcutSet)
                break;
        }

        *(*m_ListInfo + 0x04) = _processEntry;
    }

    void MakeListInfo2ItemMess()
    {
        int _itemNumArray[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        auto _fetchMenuMode = Tz::MenuBase::GetMode();
        auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());

        auto _getCurPos = Tz::CmTop::GetCurPos(0x19);

        if (_fetchMenuMode == 25)
            _getCurPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        auto _isKH1Form = _getCurPos == 0x04 && CheckKH1Form();
        auto _isShortcutSet = _getCurPos != 0x00 && _getCurPos < 0x04;

        *LS_62_type = CurPos2CurrType(_getCurPos);

        for (int i = _getCurPos == 0x00 ? 0x01 : 0x00; i < 3; i++)
        {
            auto _fetchTypeByte = *(*LS_62_type + i);
            _itemNumArray[i] = GetCustomItemNum(_fetchTypeByte);

            if (*LS_62_type == s_PlayerType - 0x04)
                break;
        }

        memset(_fetchBuffer + 0xDCE0, 0x00, 0x0360);

        auto _calcItemOffset = 0;
        auto _didProcessAI = false;

        for (int i = _getCurPos == 0x00 ? 0x01 : 0x00; i < 3; i++)
        {
            auto _fetchCurrType = *(*LS_62_type + i);

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
                        _fetchButtonIcon = s_ButtonIcon[_calcItemOffset + z];
                }

                auto _fetchListInfo = *m_ListInfo;

                if (_fetchCurrType == 0x03)
                {
                    _messageAddr = _fetchBuffer + (_calcItemOffset + z + 0x06E7) * 0x20;
                    _fetchMessage = LS_57_s_MessTbl[*(*m_ListInfo + 0x03)];
                    _didProcessAI = true;
                }

                else
                {
                    auto _fetchEntry = *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x08);
                    auto _fetchItem = YS::ITEM_TABLE::Get(_fetchEntry);
                    auto _fetchItemType = _fetchItem ? *(_fetchItem + 0x02) : -1;

                    if (_fetchEntry && (_fetchItemType == 0x12))
                    {
                        _isSpecial = *(*m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x0A) == 0x00;
                        _itemInfoStart = *m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI);
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

                if (_getCurPos && !_isKH1Form && !_isShortcutSet)
                {
                    if (_fetchCurrType == 0x02 || _fetchCurrType == 0x04)
                    {
                        auto _fetchFriend = GetFriendInfo(_fetchCurrType, *(_fetchListInfo + 0x08 * (_calcItemOffset + z) + 0x03));

                        if (_fetchFriend)
                            Tz::CmComm::FontIcon(_fetchFriend[0x04], reinterpret_cast<char*>(_fetchBuffer) + 0x04 * (_calcItemOffset + z) + 0xDFE0, true);
                    }
                }
            }

            _calcItemOffset += _itemNumArray[i];
        }
    }

    int CurPos2CustomType(int pos)
    {
        if (Tz::CmTop::GetCurPos(25) == 0x00)
            *LS_52_type = s_PlayerType;

        else if (Tz::CmTop::GetCurPos(25) < 0x04 || (Tz::CmTop::GetCurPos(25) == 0x04 && CheckKH1Form()))
            *LS_52_type = s_PlayerType - 0x04;

        else
            *LS_52_type = s_FriendType;

        auto _fetchItemCalc = 0x00;

        for (int i = Tz::CmTop::GetCurPos(25) == 0x00 ? 0x01 : 0x00; i < 3; i++)
        {
            auto _fetchItemType = *(*LS_52_type + i);
            auto _fetchItemNum = GetCustomItemNum(_fetchItemType);

            _fetchItemCalc += _fetchItemNum;

            if (pos < _fetchItemCalc)
                return _fetchItemType;
        }

        return -1;
    }
       
    void ChageAbility(int pos)
    {
        auto _fetchMode = Tz::MenuBase::GetMode();
        auto _fetchFriendInfo = GetFriendInfo(4, -1);
        auto _fetchSelectPos = Tz::CmTop::GetSelectPos(25);
        auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*m_PartyInfo, CurPos2SheetNum(_fetchSelectPos)) + 0x250);

        auto _fetchSelectAbility = Tz::CmTop::GetSelectPos(26);

        if (_fetchMode == 26 || _fetchMode == 27)
            _fetchSelectAbility = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        auto _fetchIndex = kn::FriendSaveRam::item2index(*reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _fetchSelectAbility));
        auto _friendIterator = 0x00;

        if (pos < 0x00)
        {
            while (*(*m_ListInfo + 0x08 * _fetchSelectAbility + 0x03) != *(_fetchFriendInfo + 0x08 * _friendIterator + 0x05))
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
        auto _convertSheet = PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet);

        *(_convertSheet + _fetchIndex + 0x00F5) = _fetchAbility;
        *(*m_ListInfo + 0x08 * _fetchSelectAbility + 0x03) = _fetchAbility;
    }
    
    void ChangeAutoReplenishment()
    {
        auto _fetchMode = Tz::MenuBase::GetMode();
        auto _fetchCurrentPos = Tz::CmTop::GetSelectPos(26);
        auto _fetchTopSelectPos = Tz::CmTop::GetSelectPos(25);

        auto _isKH1Form = _fetchTopSelectPos == 0x04 && CheckKH1Form();
        auto _isShortcutSet = _fetchTopSelectPos != 0x00 && _fetchTopSelectPos < 0x04;

        if (_fetchMode == 26 || _fetchMode == 27 || _fetchMode == 28)
            _fetchCurrentPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        if (_fetchTopSelectPos && !_isKH1Form && !_isShortcutSet)
            _fetchCurrentPos--;

        auto _fetchItem = *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x08);
        auto _fetchFlagCheck = *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) == 0x00;

        auto _fetchFinalFlag = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        if (_fetchTopSelectPos && !_isKH1Form && !_isShortcutSet)
        {
            if (_fetchMode != 31)
            {
                auto _fetchReplenishFlag = *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A);
                auto _fetchBehaviorFlag = *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0B);

                _fetchFinalFlag = (_fetchReplenishFlag | _fetchBehaviorFlag) + 0x01;

                if (_fetchFinalFlag >= 0x04)
                    _fetchFinalFlag = 0x00;
            }

            *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) = _fetchFinalFlag & 0x01;
            *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0B) = _fetchFinalFlag & 0x02;

            _fetchFlagCheck = *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) != 0x00;
        }

        auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*m_PartyInfo, CurPos2SheetNum(_fetchTopSelectPos)) + 0x250);
        auto _convertSheet = PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet);

        auto _fetchPartyCount = *(_convertSheet + 0x12);

        for (int i = 0; i < _fetchPartyCount; i++)
        {
            auto _fetchCurrentItemPtr = _convertSheet + 0x44 + 0x02 * i;

            auto _fetchCurrItem = *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr - 0x10);
            auto _fetchCurrReplenish = *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr);

            if (_fetchCurrItem == _fetchItem || _fetchCurrReplenish == _fetchItem)
                *reinterpret_cast<uint16_t*>(_fetchCurrentItemPtr) = _fetchFlagCheck ? _fetchItem : 0x00;
        }

        if (_fetchTopSelectPos && !_isKH1Form && !_isShortcutSet)
            *(_convertSheet + kn::FriendSaveRam::item2index(_fetchItem) + 0x0F5) = *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0B);

        if (_fetchMode == 26 || _fetchMode == 27 || _fetchMode == 28)
        {
            auto _fetchEntryCount = *(*m_ListInfo + 0x04);

            GetListInfo(_fetchTopSelectPos);
            MakeListInfo2ItemMess();

            auto _fetchEntryCountNew = *(*m_ListInfo + 0x04);

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
    
    void ChangePartyBehavior(int pos)
    {
        auto _fetchSelectPos = Tz::CmTop::GetSelectPos(25);
        auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*m_PartyInfo, CurPos2SheetNum(_fetchSelectPos)) + 0x250);
        auto _convertSheet = PC::CONVERTER::INTPTR_TO_POINTER(_fetchSheet);

        if (pos >= 0)
        {
            *(_convertSheet + 0x00F4) = pos;
            *(*m_ListInfo + 0x03) = pos;
        }

        else
        {
            auto _behaviorPtr = _convertSheet + 0x00F4;
            *_behaviorPtr += 1;

            if (*_behaviorPtr >= 0x06)
                *_behaviorPtr = 0;

            *(*m_ListInfo + 0x03) = *_behaviorPtr;
        }
    }
    
    void SetupCustom()
    {
        uint16_t IndiCustomSelSeq[] = { 0x00CA, 0x00C1, 0x00B8, 0x00A6, 0x009D };
        uint16_t IndiCustomTitleSeq[] = { 0x00CE, 0x00C5, 0x00BC, 0x00AA, 0x00A1, 0x0000 };
        uint16_t IndiCustomBaseSeq[] = { 0x00C6, 0x00C8, 0x00C7, 0x00BD, 0x00BF, 0x00BE, 0x00B4, 0x00B6, 0x00B5, 0x00A2, 0x00A4, 0x00A3, 0x0099, 0x009B, 0x009A, 0x0000 };

        int _fetchInfoMax = -1;

        auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
        auto _fetchMode = Tz::MenuBase::GetMode();
        auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
        auto _fetchCustomType = CurPos2CustomType(_fetchSelectPos);

        auto _currSeqdPtr = s_SelSeq + 0x04;

        while (_currSeqdPtr < reinterpret_cast<char*>(LS_62_type) + 0x04)
        {
            *reinterpret_cast<uint16_t*>(_currSeqdPtr - 0x04) = IndiCustomSelSeq[_fetchCustomType];
            *reinterpret_cast<uint16_t*>(_currSeqdPtr - 0x02) = IndiCustomSelSeq[_fetchCustomType];
            *reinterpret_cast<uint16_t*>(_currSeqdPtr) = IndiCustomSelSeq[_fetchCustomType];
            *reinterpret_cast<uint16_t*>(_currSeqdPtr + 0x02) = IndiCustomSelSeq[_fetchCustomType];

            _currSeqdPtr += 0x04;
        }

        auto _isKH1Form = Tz::CmTop::GetSelectPos(25) == 0x04 && CheckKH1Form();
        auto _isShortcutSet = Tz::CmTop::GetSelectPos(25) != 0x00 && Tz::CmTop::GetSelectPos(25) < 0x04;

        if (_fetchCustomType)
        {
            switch (_fetchCustomType)
            {
            case 0x01:
                _fetchInfoMax = 0x02;
                break;
            case 0x02:
                _fetchInfoMax = Tz::CmTop::GetSelectPos(25) ? 0x04 : 0x02;
                break;
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
        auto _fetchPriority = *m_pri;

        Tz::MenuUtil::CreateSprt(_fetchBuffer, _fetchPriority, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, IndiCustomBaseSeq[3 * _fetchCustomType], IndiCustomBaseSeq[3 * _fetchCustomType + 0x02], 0, 0);
        Tz::MenuUtil::CreateMess(_fetchBuffer + 0x07E0, _fetchPriority + 0x01, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, IndiCustomTitleSeq[_fetchCustomType], -1, -1, 0, 0);

        *reinterpret_cast<uint32_t*>(_fetchBuffer + 0x07F0) &= ~0x10;
        Tz::MenuUtil::SetSprtParent(_fetchBuffer + 0x07E0, _fetchBuffer);

        auto _fetchFontColor = !_isKH1Form ? Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _fetchSelectPos + 0x08), 1, _fetchCustomType == 1) : 0x00;
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

            Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, i, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(s_SelSeq), s_SelSeq + 0x08);

            auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i);
            auto _paramCr = YI::SEQUENCE::GetParamCr(_fetchItemPtr + 0x20);

            Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, i, 0, _paramCr);

            *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x10) &= ~0x10;

            if (_fetchMode == 31 && Tz::CmTop::GetSelectPos(25) >= 0x04 && !_isKH1Form)
                *reinterpret_cast<char**>(Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i) + 0x220) = DrawItemAILv;

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

        SetIndiCustomDefaultPos();

        Tz::MenuBase::SetSMode(0);
        UpdateIndiCustomList();
    }
    
    void UpdateCustomList()
    {
        int _itemNumArray[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        uint16_t _listSelSeq[] = { 0x0108, 0x0107, 0x0106, 0x00FA, 0x00F8, 0x0000, 0x0000, 0x0000 };
        uint16_t _listSubSeq[] = { 0x011A, 0x0119, 0x0118, 0x0115, 0x0114, 0x0000 };
        uint16_t _listSubMsg[] = { 0x847D, 0x8481, 0x8482, 0x8483, 0x8484, 0x0000 };

        uint16_t _listShortcutSubMsg[] = { 0x847D, 0x847E, 0x847F, 0x8480 };

        auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
        auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);

        auto _fetchCurPos = Tz::CmTop::GetCurPos(25);

        auto _isKH1Form = _fetchCurPos == 0x04 && CheckKH1Form();
        auto _isShortcutSet = _fetchCurPos != 0x00 && _fetchCurPos < 0x04;

        *LS_45_type = CurPos2CurrType(_fetchCurPos);

        if ((_isKH1Form || _isShortcutSet) && *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x16) != 0x04)
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

        for (int i = _fetchCurPos == 0x00 ? 0x01 : 0x00; i < 0x03; i++)
        {
            auto _fetchTypeByte = *(*LS_45_type + i);
            _itemNumArray[i] = GetCustomItemNum(_fetchTypeByte);

            if (_isKH1Form || _isShortcutSet)
                break;
        }

        auto _fetchMaxLoop = *(*m_ListInfo + 0x04);
        auto _itemNumberCalc = 0x00;

        auto _processEntry = 0x00;

        if (_fetchCurPos && !_isKH1Form && !_isShortcutSet)
            _fetchMaxLoop++;

        if (_fetchMaxLoop > 0x09)
            _fetchMaxLoop = 0x09;

        for (int i = _fetchCurPos == 0x00 ? 0x01 : 0x00; i < 0x03; i++)
        {
            auto _fetchTypeByte = *(*LS_45_type + i);

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
                            _fetchFontColor = Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * (_itemNumberCalc + z) + 0x08), 0x01, _fetchTypeByte == 0x01);

                        _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);
                        dk::Sprite::setNum(_fetchBuffer + 0x220 * _processEntry + 0x7660, _fetchFontColorSeq);
                        *reinterpret_cast<char**>(_fetchMessagePtr) = _fetchMessage;

                        _processEntry++;
                        _fetchMessagePtr += 0x220;
                    }
                }
            }

            if (_isKH1Form || _isShortcutSet)
                break;

            _itemNumberCalc += _itemNumArray[i];
        }
    }
    
    void ChangeCustomInfo()
    {
        map<uint16_t, uint16_t> _limitShortcutMap =
        {
            { 0x0239, 0x02BA },
            { 0x023A, 0x02BD },
            { 0x023B, 0x02C0 },
            { 0x023C, 0x02AB }
        };

        auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
        auto _fetchCustomType = CurPos2CustomType(_fetchSelectPos);
        auto _fetchMenuSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        auto _isKH1Form = Tz::CmTop::GetCurPos(25) == 0x04 && CheckKH1Form();
        auto _isShortcutSet = Tz::CmTop::GetCurPos(25) != 0x00 && Tz::CmTop::GetCurPos(25) < 0x04;

        auto _fetchCommand = 0x00;

        switch (_fetchCustomType)
        {
        case 4:
            ChageAbility(_fetchMenuSelectPos);
            break;
        case 3:
            ChangePartyBehavior(_fetchMenuSelectPos);
            break;
        case 2:
        {
            if (Tz::CmTop::GetSelectPos(25))
                _fetchSelectPos--;

            auto _fetchReplenishFlag = *(*m_ListInfo + 0x08 * _fetchSelectPos + 0x0A) != 0x00;
            auto _fetchBehaviorFlag = *(*m_ListInfo + 0x08 * _fetchSelectPos + 0x0B) == 0x02;
            auto _fetchOffset = _fetchReplenishFlag;

            if (_fetchBehaviorFlag && Tz::CmTop::GetSelectPos(25))
                _fetchOffset += 2;

            if (_fetchOffset != _fetchMenuSelectPos);
            ChangeAutoReplenishment();
        } break;
        case 1:
        {
            auto _fetchMpFlag = m_ListInfo + 0x08 * _fetchSelectPos + 0x0A != 0x00;

            if (_fetchMpFlag != _fetchMenuSelectPos);
            ChageMpDrive();
        } break;
        default:
        {
            if (_fetchMenuSelectPos)
            {
                if (!_isKH1Form)
                {
                    auto _fetchInfo = *Tz::CmTop::m_ItemInfo + 0x04;
                    _fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 1)));
                }

                else
                {
                    auto _fetchInfo = *Tz::CmTop::m_ItemInfo + 0x04;

                    _fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 1)));

                    if (_fetchCommand == 0x0000)
                        _fetchCommand = _limitShortcutMap[*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 1))];
                }
            }

            auto _shortcutPtr = _isKH1Form ? AREA::SaveData + 0x371C : AREA::SaveData + 0x10108 + ((Tz::CmTop::GetCurPos(25) - 0x01) * 0x08);
            *reinterpret_cast<uint16_t*>(_shortcutPtr + (_fetchSelectPos * 0x02)) = _fetchCommand;

        } break;
        }

        memcpy(LS_KH1F_Shortcuts, AREA::SaveData + 0x371C, 0x08);

        if (Tz::CmTop::GetCurPos(25) - 1 == *(AREA::SaveData + 0x10104))
            memcpy(AREA::SaveData + 0x36F8, AREA::SaveData + 0x10108 + (Tz::CmTop::GetCurPos(25) - 1) * 0x08, 0x08);

        GetListInfo(Tz::CmTop::GetSelectPos(25));
    }
    
    void UpdateTopList()
    {
        int _itemNumArray[0x06] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        auto _fetchBuffer = Tz::CmTop::GetListBuffer();
        auto _fetchCurrent = **Tz::CmTop::m_MenuPtr;

        GetListInfo(_fetchCurrent);
        MakeListInfo2ItemMess();

        auto _fetchSheetNum = CurPos2SheetNum(_fetchCurrent);
        *LS_103_type = CurPos2CurrType(_fetchCurrent);

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

        auto _fetchListInfo = *m_ListInfo;
        auto _calcItemOffset = 0x00;

        for (int i = _fetchCurrent == 0x00 ? 0x01 : 0x00; i < 0x03; i++)
        {
            auto _fetchTypeByte = *(*LS_103_type + i);
            _itemNumArray[i] = GetCustomItemNum(_fetchTypeByte);

            if (*LS_103_type == s_PlayerType - 0x04)
                break;
        }

        for (int i = _fetchCurrent == 0x00 ? 0x01 : 0x00; i < 0x03; i++)
        {
            auto _itemIndex = *(*LS_103_type + i);
            auto _fetchSeq = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint64_t>(LS_103_s_Seq) + 0x02 * _itemIndex);

            if (_itemNumArray[i])
            {
                for (int z = 0; z < _itemNumArray[i]; z++)
                {
                    auto _fetchBufferIndex = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * _calcItemOffset + 0x1258 + 0x220 * z;
                    auto _currOffset = _calcItemOffset - i;

                    auto _calcSeq = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * (_calcItemOffset + z) + 0x4360;
                    dk::Sprite::setNum(_calcSeq, _fetchSeq);

                    auto _fetchFontColor = 0x00;

                    if (_itemIndex != 0x03 && *LS_103_type != s_PlayerType - 0x04)
                        _fetchFontColor = Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * (_calcItemOffset + z) + 0x08), 0x01, _itemIndex == 0x01);

                    auto _fetchFontColorSqd = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);
                    auto _calcFontSeq = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * (_calcItemOffset + z) + 0x7660;

                    dk::Sprite::setNum(_calcFontSeq, _fetchFontColorSqd);
                    *reinterpret_cast<char**>(_fetchBufferIndex + 0x6600) = reinterpret_cast<char*>(_fetchBuffer) + 32 * (_calcItemOffset + z + 0x6E7);

                    if (*LS_103_type == s_FriendType && _itemIndex != 0x03)
                        *reinterpret_cast<char**>(_fetchBufferIndex) = reinterpret_cast<char*>(_fetchBuffer) + 0x04 * (_calcItemOffset + z) + 0xDFE0;
                }

                _calcItemOffset = _calcItemOffset + _itemNumArray[i];
            }
        }
    }

    void SetupTop()
    {
        auto _fetchBuffer = Tz::CmTop::GetListBuffer();
        Tz::SelHist::Create(0, -1);

        auto _fetchPriority = *m_pri;
        auto _processIdx = 0x00;
        *m_pri = 30017;

        for (int i = 0; i < 0x04; i++)
        {
            if (Tz::PartyInfo::GetSheet(*m_PartyInfo, i))
            {
                auto _fetchPlateZero = Tz::CmTop::GetTopPlateSeqTbl(0);
                auto _fetchPlateFirst = Tz::CmTop::GetTopPlateSeqTbl(1);

                Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlateZero), _fetchPlateFirst);
                Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx, 0, -1);

                auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx);

                *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
                *reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = Tz::PartyInfo::GetName(*m_PartyInfo, i);
                *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

                if (i == 0x00)
                {
                    auto _fetchPlate2Zero = Tz::CmTop::GetTopPlateSeqTbl(0) + 0x10;
                    auto _fetchPlate2First = Tz::CmTop::GetTopPlateSeqTbl(1) + 0x10;

                    for (int z = 0x00; z < 0x03; z++)
                    {
                        Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx + 1, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlate2Zero), _fetchPlate2First);
                        Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx + 1, 0, -1);

                        auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx + 1);

                        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
                        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

                        *reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = YS::MESSAGE::GetData(0x575D + z);

                        _processIdx++;
                    }

                    if (CheckKH1Form())
                    {
                        Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx + 1, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlate2Zero), _fetchPlate2First);
                        Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx + 1, 0, -1);

                        auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx + 1);

                        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
                        *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

                        *reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = YS::MESSAGE::GetData(0x4E80);

                        _processIdx++;
                    }
                }

                _processIdx++;
            }
        }

        auto _fetchCurrPos = Tz::CmTop::GetCurPos(25);
        Tz::Select::SetCurrent(*Tz::CmTop::m_MenuPtr, _fetchCurrPos, true, 0x00, 0x02);
        Tz::MenuBase::SetSMode(0x00);
        *m_pri = _fetchPriority;
        CreateTopList();
        UpdateTopList();
    }
        
    void UpdateHelpMess()
    {
        map<uint16_t, uint16_t> _limitShortcutMap =
        {
            { 0x02BA, 0x0239 },
            { 0x02BD, 0x023A },
            { 0x02C0, 0x023B },
            { 0x02AB, 0x023C }
        };

        uint16_t _magicHelpText[] = { 0x9DFB, 0x9DFA };
        uint16_t _itemHelpText[] = { 0x9DFF, 0x9DFE };
        uint16_t _itemFreqHelpText[] = { 0x9E5E, 0x9E5F };

        auto _fetchMenuMode = Tz::MenuBase::GetMode();
        auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
        auto _fetchSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        auto _isKH1Form = ((_fetchMenuMode == 25 && _fetchSelectPos == 0x04) || (_fetchMenuMode != 25 && Tz::CmTop::GetCurPos(25) == 0x04)) && CheckKH1Form();
        auto _isShortcutSet = (_fetchMenuMode == 25 && _fetchSelectPos != 0x00 && _fetchSelectPos < 0x04) || (_fetchMenuMode != 25 && Tz::CmTop::GetCurPos(25) != 0x00 && Tz::CmTop::GetCurPos(25) < 0x04);

        uint16_t _messageId = UINT16_MAX;
        uint16_t _sideMessageId = UINT16_MAX;

        switch (_fetchMenuMode)
        {
        case 25:
        {
            if (_fetchSelectPos == 0x00)
                _messageId = 0x5765;

            else
                _messageId = _isShortcutSet ? 0x5760 : (_isKH1Form ? 0x575C : 0x3B8A);
        } break;

        case 26:
        case 27:
        case 28:
        {
            auto _fetchCustomType = CurPos2CustomType(_fetchSelectPos);

            if (_fetchCustomType == 0x03)
                _messageId = *reinterpret_cast<uint16_t*>(GetFriendInfo(0x03, *(*m_ListInfo + 3)) + 0x02);

            else
            {
                auto _fetchMainPos = Tz::CmTop::GetCurPos(25);

                if (_fetchMainPos && !_isKH1Form && !_isShortcutSet)
                    _fetchSelectPos -= 1;

                auto _fetchItemId = *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _fetchSelectPos + 0x08);

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

                if (_fetchMenuMode == 27 && !_isKH1Form && !_isShortcutSet)
                    _sideMessageId = *reinterpret_cast<uint16_t*>(GetFriendInfo(_fetchCustomType, *(*m_ListInfo + 0x08 * _fetchSelectPos + 0x0B)) + 0x02);
            }
        } break;
        case 29:
        {
            if (_fetchSelectPos > 0x00)
            {
                auto _itemInfo = reinterpret_cast<char*>(*Tz::CmTop::m_ItemInfo + 0x04);
                auto _fetchItemId = *reinterpret_cast<uint16_t*>(_itemInfo + 0x06 * (_fetchSelectPos - 0x01));
                auto _fetchItemEntry = YS::ITEM_TABLE::Get(_fetchItemId);

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

            if (_fetchMainPos && !_isKH1Form && !_isShortcutSet)
                _sideMessageId = _itemFreqHelpText[_fetchSelectPos / 0x02];
        } break;

        default:
            _messageId = *reinterpret_cast<uint16_t*>(GetFriendInfo(_fetchMenuMode == 32 ? 0x03 : 0x04, Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr)) + 0x02);
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
    
    bool isTakeOff()
    {
        auto _fetchMode = Tz::MenuBase::GetMode();
        auto _fetchCurrentPos = Tz::CmTop::GetSelectPos(26);
        auto _fetchTopSelectPos = Tz::CmTop::GetSelectPos(25);

        auto _isKH1Form = _fetchTopSelectPos == 0x04 && CheckKH1Form();
        auto _isShortcutSet = _fetchTopSelectPos != 0x00 && _fetchTopSelectPos < 0x04;

        if (_fetchMode == 26 || _fetchMode == 27)
            _fetchCurrentPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

        if (_fetchTopSelectPos && !_isKH1Form && !_isShortcutSet)
            _fetchCurrentPos--;

        auto _fetchItem = *reinterpret_cast<uint16_t*>(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x08);
        auto _fetchFlagCheck = *(*m_ListInfo + 0x08 * _fetchCurrentPos + 0x0A) != 0x00;

        if (_fetchMode != 26 && _fetchMode != 27)
            _fetchFlagCheck = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr) % 0x02 == 0x00;

        if (!_fetchFlagCheck)
            return false;

        auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*m_PartyInfo, CurPos2SheetNum(_fetchTopSelectPos)) + 0x250);
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

    __declspec(dllexport) const char* RF_ExclusivityTags()
    {
        return "_kh2ShortcutSets";
    }
    
    __declspec(dllexport) void RF_ModuleInit(const wchar_t* mod_path)
    {
        wchar_t filepath[MAX_PATH];

        wcscpy_s(filepath, mod_path);
        wcscat_s(filepath, L"\\dll\\ReFined.KH2.dll");

        MAIN_HANDLE = LoadLibraryW(filepath);

        if (MAIN_HANDLE)
        {
            // ====================================================================================================================== //

            AREA::Current = reinterpret_cast<AREA::INFO*>(*(void**)GetProcAddress(MAIN_HANDLE, "?Current@AREA@@2PEAUINFO@1@EA"));
            AREA::SaveData = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@@2PEADEA"));
            AREA::IsInMap = reinterpret_cast<bool*>(*(void**)GetProcAddress(MAIN_HANDLE, "?IsInMap@AREA@@2PEA_NEA"));

            // ====================================================================================================================== //

            SOUND::PlaySFX = reinterpret_cast<void(*)(uint32_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?PlaySFX@SOUND@@2P6AXI@ZEA"));

            // ====================================================================================================================== //

            dk::COMMAND_DRAW::CommandMenu = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?CommandMenu@COMMAND_DRAW@dk@@2PEAPEADEA"));
            
            // ====================================================================================================================== //

            dk::Sprite::setNum = reinterpret_cast<void(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?setNum@Sprite@dk@@2P6AXPEADH@ZEA"));

            // ====================================================================================================================== //

            kn::FriendSaveRam::item2index = reinterpret_cast<int(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?item2index@FriendSaveRam@kn@@2P6AHH@ZEA"));

            // ====================================================================================================================== //

            PC::CONVERTER::INTPTR_TO_POINTER = reinterpret_cast<char* (*)(uint32_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?INTPTR_TO_POINTER@CONVERTER@PC@@2P6APEADI@ZEA"));

            // ====================================================================================================================== //

            Tz::CmComm::FontIcon = reinterpret_cast<void(*)(char, char*, bool)>(*(void**)GetProcAddress(MAIN_HANDLE, "?FontIcon@CmComm@Tz@@2P6AXDPEAD_N@ZEA"));
            Tz::CmComm::ItemIcon = reinterpret_cast<void(*)(int, char*, bool, bool)>(*(void**)GetProcAddress(MAIN_HANDLE, "?ItemIcon@CmComm@Tz@@2P6AXHPEAD_N1@ZEA"));

            // ====================================================================================================================== //

            Tz::CMenuHelp::Create = reinterpret_cast<void(*)(char*, bool)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Create@CMenuHelp@Tz@@2P6AXPEAD_N@ZEA"));

            // ====================================================================================================================== //

            Tz::CmTop::GetListBuffer = reinterpret_cast<char**(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetListBuffer@CmTop@Tz@@2P6APEAPEADXZEA"));
            Tz::CmTop::GetCurPos = reinterpret_cast<int(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetCurPos@CmTop@Tz@@2P6AHH@ZEA"));
            Tz::CmTop::GetScrBarSeqTbl = reinterpret_cast<char* (*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetScrBarSeqTbl@CmTop@Tz@@2P6APEADXZEA"));
            Tz::CmTop::GetSelectPos = reinterpret_cast<int(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSelectPos@CmTop@Tz@@2P6AHH@ZEA"));
            Tz::CmTop::GetFontColorSeqNum = reinterpret_cast<int(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetFontColorSeqNum@CmTop@Tz@@2P6AHH@ZEA"));
            Tz::CmTop::GetTopPlateSeqTbl = reinterpret_cast<char* (*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetTopPlateSeqTbl@CmTop@Tz@@2P6APEADH@ZEA"));
            Tz::CmTop::GetItemFontColor = reinterpret_cast<int(*)(int, int, bool)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetItemFontColor@CmTop@Tz@@2P6AHHH_N@ZEA"));

            Tz::CmTop::m_SeqUnit = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_SeqUnit@CmTop@Tz@@2PEAHEA"));
            Tz::CmTop::m_ImgUnit = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_ImgUnit@CmTop@Tz@@2PEAHEA"));
            Tz::CmTop::m_DummySeq = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_DummySeq@CmTop@Tz@@2PEAHEA"));

            Tz::CmTop::m_Lay = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_Lay@CmTop@Tz@@2PEAPEADEA"));
            Tz::CmTop::m_SclBar = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_SclBar@CmTop@Tz@@2PEAPEADEA"));
            Tz::CmTop::m_MenuPtr = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_MenuPtr@CmTop@Tz@@2PEAPEADEA"));
            Tz::CmTop::m_ItemInfo = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_ItemInfo@CmTop@Tz@@2PEAPEADEA"));

            // ====================================================================================================================== //

            Tz::ItemInfo::SetItemInfo = reinterpret_cast<void(*)(char*, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetItemInfo@ItemInfo@Tz@@2P6AXPEADHH@ZEA"));
            Tz::ItemInfo::SortItemInfo = reinterpret_cast<void(*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SortItemInfo@ItemInfo@Tz@@2P6AXPEAD@ZEA"));
            Tz::ItemInfo::MakeEmptyMsg = reinterpret_cast<void(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?MakeEmptyMsg@ItemInfo@Tz@@2P6AXPEADH@ZEA"));
            Tz::ItemInfo::MakeCommandMsg = reinterpret_cast<int(*)(char*, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?MakeCommandMsg@ItemInfo@Tz@@2P6AHPEADHH@ZEA"));
            Tz::ItemInfo::AddKnowItemInfo = reinterpret_cast<void(*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?AddKnowItemInfo@ItemInfo@Tz@@2P6AXPEAD@ZEA"));

            // ====================================================================================================================== //

            Tz::MenuBase::GetMode = reinterpret_cast<int(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetMode@MenuBase@Tz@@2P6AHXZEA"));
            Tz::MenuBase::SetSMode = reinterpret_cast<void(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetSMode@MenuBase@Tz@@2P6AXH@ZEA"));

            // ====================================================================================================================== //

            Tz::MenuUtil::CreateMess = reinterpret_cast<char* (*)(char*, int, uint32_t, uint32_t, char*, int, uint32_t, int, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?CreateMess@MenuUtil@Tz@@2P6APEADPEADHII0HIHHH@ZEA"));
            Tz::MenuUtil::CreateSprt = reinterpret_cast<char* (*)(char*, int, uint32_t, uint32_t, char*, int, uint32_t, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?CreateSprt@MenuUtil@Tz@@2P6APEADPEADHII0HIHH@ZEA"));
            Tz::MenuUtil::SetSprtParent = reinterpret_cast<void(*)(char*, char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetSprtParent@MenuUtil@Tz@@2P6AXPEAD0@ZEA"));

            // ====================================================================================================================== //

            Tz::PartyInfo::GetName = reinterpret_cast<char* (*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetName@PartyInfo@Tz@@2P6APEADPEADH@ZEA"));
            Tz::PartyInfo::GetSheet = reinterpret_cast<char* (*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSheet@PartyInfo@Tz@@2P6APEADPEADH@ZEA"));

            // ====================================================================================================================== //

            Tz::ScrollBar::Create = reinterpret_cast<void(*)(char*, int, char*, char*, char*, int, int, char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Create@ScrollBar@Tz@@2P6AXPEADH000HH0@ZEA"));
            Tz::ScrollBar::SetMax = reinterpret_cast<void(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetMax@ScrollBar@Tz@@2P6AXPEADH@ZEA"));
            Tz::ScrollBar::SetPos = reinterpret_cast<void(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetPos@ScrollBar@Tz@@2P6AXPEADH@ZEA"));

            // ====================================================================================================================== //

            Tz::Select::SetItem = reinterpret_cast<void(*)(char*, int, uint32_t, uint32_t, char*, int, uint16_t*, char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetItem@Select@Tz@@2P6AXPEADHII0HPEAG1@ZEA"));
            Tz::Select::GetItemPtr = reinterpret_cast<char* (*)(char*, uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetItemPtr@Select@Tz@@2P6APEADPEAD_K@ZEA"));
            Tz::Select::SetCurrent = reinterpret_cast<void(*)(char*, int, bool, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetCurrent@Select@Tz@@2P6AXPEADH_NHH@ZEA"));
            Tz::Select::SetExOffset = reinterpret_cast<void(*)(char*, int, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetExOffset@Select@Tz@@2P6AXPEADHHH@ZEA"));
            Tz::Select::SetPageMode = reinterpret_cast<void(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetPageMode@Select@Tz@@2P6AXPEADH@ZEA"));
            Tz::Select::GetSelectPos = reinterpret_cast<int(*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSelectPos@Select@Tz@@2P6AHPEAD@ZEA"));
            Tz::Select::SetSelectMax = reinterpret_cast<void(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetSelectMax@Select@Tz@@2P6AXPEADH@ZEA"));
            Tz::Select::SetSelectTop = reinterpret_cast<int(*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetSelectTop@Select@Tz@@2P6AHPEADH@ZEA"));

            // ====================================================================================================================== //

            Tz::SelHist::Create = reinterpret_cast<void(*)(int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Create@SelHist@Tz@@2P6AXHH@ZEA"));

            // ====================================================================================================================== //

            YI::IMAGE_FREEZE::GetImage = reinterpret_cast<char* (*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetImage@IMAGE_FREEZE@YI@@2P6APEADPEADH@ZEA"));

            // ====================================================================================================================== //

            YI::LAYOUT::GetSequenceUnit = reinterpret_cast<char* (*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSequenceUnit@LAYOUT@YI@@2P6APEADPEADH@ZEA"));

            // ====================================================================================================================== //

            YI::SEQUENCE::GetParamCr = reinterpret_cast<int(*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetParamCr@SEQUENCE@YI@@2P6AHPEAD@ZEA"));

            // ====================================================================================================================== //

            YS::COMMAND_ELEM::Get = reinterpret_cast<char* (*)(uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Get@COMMAND_ELEM@YS@@2P6APEAD_K@ZEA"));

            // ====================================================================================================================== //

            YS::HARDPAD::Input = reinterpret_cast<uint16_t*>(*(void**)GetProcAddress(MAIN_HANDLE, "?Input@HARDPAD@YS@@2PEAGEA"));

            // ====================================================================================================================== //

            YS::ITEM::GetCommand = reinterpret_cast<int(*)(uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetCommand@ITEM@YS@@2P6AH_K@ZEA"));
            YS::ITEM::GetNumBackyard = reinterpret_cast<uint64_t(*)(uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetNumBackyard@ITEM@YS@@2P6A_K_K@ZEA"));

            // ====================================================================================================================== //

            YS::ITEM_TABLE::Get = reinterpret_cast<char* (*)(uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Get@ITEM_TABLE@YS@@2P6APEAD_K@ZEA"));
            YS::ITEM_TABLE::Each = reinterpret_cast<char* (*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Each@ITEM_TABLE@YS@@2P6APEADPEAD@ZEA"));

            // ====================================================================================================================== //

            YS::MAGIC::GetMpDriveStatus = reinterpret_cast<bool(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetMpDriveStatus@MAGIC@YS@@2P6A_NH@ZEA"));

            // ====================================================================================================================== //

            MENU::IsMenu = reinterpret_cast<bool*>(*(void**)GetProcAddress(MAIN_HANDLE, "?IsMenu@MENU@@2PEA_NEA"));

            // ====================================================================================================================== //

            YS::MESSAGE::GetSize = reinterpret_cast<const size_t(*)(const char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSize@MESSAGE@YS@@2P6A?B_KPEBD@ZEA"));
            YS::MESSAGE::GetData = reinterpret_cast<char* (*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetData@MESSAGE@YS@@2P6APEADH@ZEA"));

            // ====================================================================================================================== //
           
            YS::TITLE::IsTitle = reinterpret_cast<bool*>(*(void**)GetProcAddress(MAIN_HANDLE, "?IsTitle@TITLE@YS@@2PEA_NEA"));
            
            // ====================================================================================================================== //

            CAN_ALTER_KH1F = reinterpret_cast<bool>(*(void**)GetProcAddress(MAIN_HANDLE, "?CAN_ALTER_KH1F@CmCustom@Tz@@2_NA"));

            m_pri = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_pri@CmCustom@Tz@@2PEAHEA"));

            s_SelSeq = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?s_SelSeq@CmCustom@Tz@@2PEADEA"));
            s_PlayerType = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?s_PlayerType@CmCustom@Tz@@2PEADEA"));
            s_FriendType = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?s_FriendType@CmCustom@Tz@@2PEADEA"));
            s_ButtonIcon = reinterpret_cast<short*>(*(void**)GetProcAddress(MAIN_HANDLE, "?s_ButtonIcon@CmCustom@Tz@@2PEAFEA"));

            m_ListInfo = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_ListInfo@CmCustom@Tz@@2PEAPEADEA"));
            m_PartyInfo = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_PartyInfo@CmCustom@Tz@@2PEAPEADEA"));

            LS_45_type = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_45_type@CmCustom@Tz@@2PEAPEADEA"));
            LS_52_type = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_52_type@CmCustom@Tz@@2PEAPEADEA"));
            LS_62_type = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_62_type@CmCustom@Tz@@2PEAPEADEA"));
            LS_103_type = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_103_type@CmCustom@Tz@@2PEAPEADEA"));

            LS_103_s_Seq = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_103_s_Seq@CmCustom@Tz@@2PEADEA"));
            LS_57_s_MessTbl = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_57_s_MessTbl@CmCustom@Tz@@2PEAHEA"));

            LS_KH1F_Shortcuts = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?LS_KH1F_Shortcuts@CmCustom@Tz@@2PEADEA"));

            DrawItemAILv = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?DrawItemAILv@CmCustom@Tz@@2PEADEA"));

            ChageMpDrive = reinterpret_cast<void(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?ChageMpDrive@CmCustom@Tz@@2P6AXXZEA"));
            CreateTopList = reinterpret_cast<void(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?CreateTopList@CmCustom@Tz@@2P6AXXZEA"));
            UpdateIndiCustomList = reinterpret_cast<void(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?UpdateIndiCustomList@CmCustom@Tz@@2P6AXXZEA"));
            SetIndiCustomDefaultPos = reinterpret_cast<void(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetIndiCustomDefaultPos@CmCustom@Tz@@2P6AXXZEA"));

            GetFriendInfo = reinterpret_cast<char* (*)(int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetFriendInfo@CmCustom@Tz@@2P6APEADHH@ZEA"));

            RedirectFunctionSLIM("?CurPos2CustomType@CmCustom@Tz@@SAHH@Z", reinterpret_cast<uint64_t>(CurPos2CustomType));
            RedirectFunctionSLIM("?GetCustomItemNum@CmCustom@Tz@@SAHH@Z", reinterpret_cast<uint64_t>(GetCustomItemNum));
            RedirectFunctionSLIM("?ChageAbility@CmCustom@Tz@@SAXH@Z", reinterpret_cast<uint64_t>(ChageAbility));
            RedirectFunctionSLIM("?ChangeAutoReplenishment@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(ChangeAutoReplenishment));
            RedirectFunctionSLIM("?ChangePartyBehavior@CmCustom@Tz@@SAXH@Z", reinterpret_cast<uint64_t>(ChangePartyBehavior));
            RedirectFunctionSLIM("?SetupCustom@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(SetupCustom));
            RedirectFunctionSLIM("?UpdateCustomList@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(UpdateCustomList));
            RedirectFunctionSLIM("?ChangeCustomInfo@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(ChangeCustomInfo));
            RedirectFunctionSLIM("?GetListInfo@CmCustom@Tz@@SAXH@Z", reinterpret_cast<uint64_t>(GetListInfo));
            RedirectFunctionSLIM("?MakeListInfo2ItemMess@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(MakeListInfo2ItemMess));
            RedirectFunctionSLIM("?SetupTop@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(SetupTop));
            RedirectFunctionSLIM("?UpdateTopList@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(UpdateTopList));
            RedirectFunctionSLIM("?UpdateHelpMess@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(UpdateHelpMess));
            RedirectFunctionSLIM("?isTakeOff@CmCustom@Tz@@SA_NXZ", reinterpret_cast<uint64_t>(isTakeOff));
            RedirectFunctionSLIM("?CheckKH1Form@CmCustom@Tz@@SA_NXZ", reinterpret_cast<uint64_t>(CheckKH1Form));

            // ====================================================================================================================== //
        }
    }

    __declspec(dllexport) void RF_ModuleExecute()
    {
        if (*YS::TITLE::IsTitle && SYNC_SHORTCUTS)
            SYNC_SHORTCUTS = false;

        else if (*AREA::IsInMap)
        {
            if (!SYNC_SHORTCUTS)
                memcpy(AREA::SaveData + 0x36F8, AREA::SaveData + 0x10108 + (*(AREA::SaveData + 0x10104) * 0x08), 0x08);

            bool _fetchInput = (*YS::HARDPAD::Input & 0x0010) == 0x0010 || (*YS::HARDPAD::Input & 0x0040) == 0x0040;
            auto _commandTypePtr = *dk::COMMAND_DRAW::CommandMenu ? reinterpret_cast<uint8_t*>(*dk::COMMAND_DRAW::CommandMenu) : nullptr;

            // If the command type pointer is not null;
            if (_commandTypePtr != nullptr && !*MENU::IsMenu)
            {
                if ((*_commandTypePtr == 0x05 && !_fetchInput) || *_commandTypePtr != 0x05)
                    DEBOUNCE_SHORTCUT = false;

                if (!DEBOUNCE_SHORTCUT)
                {
                    if (*_commandTypePtr == 0x05)
                    {
                        auto _flowDirection = (*YS::HARDPAD::Input & 0x0010) == 0x0010 ? -1 : ((*YS::HARDPAD::Input & 0x0040) == 0x0040 ? 1 : 0);

                        if (*(AREA::SaveData + 0x3524) != 0x03 && *(AREA::SaveData + 0x3524) != 0x06)
                        {
                            if (_flowDirection != 0x00)
                            {
                                SOUND::PlaySFX(0x14);

                                if (_flowDirection == -1 && *(AREA::SaveData + 0x10104) == 0x00)
                                    *(AREA::SaveData + 0x10104) = 0x02;

                                else if (_flowDirection == 1 && *(AREA::SaveData + 0x10104) == 0x02)
                                    *(AREA::SaveData + 0x10104) = 0x00;

                                else
                                    *(AREA::SaveData + 0x10104) = _flowDirection == 1 ? (*(AREA::SaveData + 0x10104) + 1) : (*(AREA::SaveData + 0x10104) - 1);

                                memcpy(AREA::SaveData + 0x36F8, AREA::SaveData + 0x10108 + (*(AREA::SaveData + 0x10104) * 0x08), 0x08);

                                DEBOUNCE_SHORTCUT = true;
                            }
                        }

                        // If we are and still have input;
                        else if (_flowDirection != 0x00)
                        {
                            SOUND::PlaySFX(0x05);
                            DEBOUNCE_SHORTCUT = true;
                        }
                    }
                }
            }
        }
    }
}