#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <thread>
#include <Windows.h>
#include <Psapi.h>

using namespace std;

// memorymgr implementation //

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

	VirtualProtect(_fetchFunction, 4096, PAGE_EXECUTE_READWRITE, &_oldProtect);

	memset(_fetchFunction, 0x90, _absoluteInstructionJMP.size());

	memcpy(_absoluteInstructionJMP.data() + 0x06, &function, 0x08);
	memcpy(_fetchFunction, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
}

template <typename T>
T FindSignature(const char* pattern, const char* mask)
{
	auto _patternLen = std::strlen(mask);
	auto _maxSearch = moduleInfo.moduleEnd - _patternLen;

	for (char* _currPtr = moduleInfo.moduleStart; _currPtr < _maxSearch; ++_currPtr)
	{
		size_t _procBytes = 0;

		for (; _procBytes < _patternLen; ++_procBytes)
		{
			if (mask[_procBytes] != '?' && pattern[_procBytes] != _currPtr[_procBytes])
				break;
		}

		if (_procBytes == _patternLen)
			return reinterpret_cast<T>(_currPtr);
	}

	return nullptr;
}

// ========================= //

bool SUBMIT_SHORTCUTS = false;
bool REVERTED_SHORTNAME = false;
bool DEBOUNCE_SHORTCUT = false;

bool EDITING_SHORTCUT_NAME = false;

string EDIT_NAME = "";
uint8_t CURRENT_KEY_SHORTCUT = 0x30;

vector<uint8_t> KEY_DEBOUNCE(256);
uint8_t KEY_DEBOUNCE_TIMEOUT = 0x00;
uint16_t KEY_ENTER_TIMEOUT = 0x00;

uint8_t CURRENT_SHORTCUT_SET = 0x80;
string DEFAULT_SHORTCUT;

vector<uint8_t> INPUT_REG;
char* OFFSET_INPUT_REG = FindSignature<char*>("\x40\x53\x55\x41\x54\x41\x55\x41\x56\x48\x83\xEC\x30\x48\x8B\x2D", "xxxxxxxxxxxxxxxx");

class AREA
{
public:
	static inline char* SaveData;
};

namespace Tz
{
	class CmCustom
	{
	public:
		static inline bool  (*CheckKH1Form)();
		static inline bool  (*CreateTopList)();
		static inline bool  (*UpdateTopList)();
		static inline int   (*CurPos2CustomType)(int num);
		static inline char* (*GetFriendInfo)(int type, int num);

		static inline int* m_pri;
		static inline char** m_ListInfo;
		static inline char** m_PartyInfo;
	};

	class CMenuHelp
	{
	public:
		static inline void (*Create)(char* message, bool param_xy);
	};

	class CmTop
	{
	public:
		static inline char** (*GetListBuffer)();

		static inline int    (*GetCurPos)(int num);
		static inline int    (*GetSelectPos)(int num);
		static inline char* (*GetTopPlateSeqTbl)(int num);

		static inline int* m_SeqUnit;
		static inline int* m_ImgUnit;
		static inline int* m_DummySeq;

		static inline char** m_Lay;
		static inline char** m_MenuPtr;
		static inline char** m_ItemInfo;
	};

	class MenuBase
	{
	public:
		static inline int  (*GetMode)();
		static inline void (*SetSMode)(int mode);
	};

	class PartyInfo
	{
	public:
		static inline char* (*GetName)(char* partyInfo, int num);
		static inline char* (*GetSheet)(char* partyInfo, int num);
	};

	class Select
	{
	public:
		static inline int   (*GetSelectPos)(char* select);
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

namespace YS
{
	class COMMAND_ELEM
	{
	public:
		static inline char* (*Get)(uint64_t id);
	};

	class ITEM_TABLE
	{
	public:
		static inline char* (*Each)(char* itemTable);
		static inline char* (*Get)(uint64_t itemId);
	};

	class MEMBER_TABLE
	{
	public:
		static inline char* MemberTable;
	};

	class MESSAGE
	{
	public:
		static inline char* (*GetData)(int id);
		static inline const size_t(*GetSize)(const char* message);
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
				_messageId = 0x5761;

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

	__declspec(dllexport) void RF_ModuleInit(const wchar_t* mod_path)
	{
		wchar_t filepath[MAX_PATH];

		wcscpy_s(filepath, mod_path);
		wcscat_s(filepath, L"\\dll\\ReFined.KH2.dll");

		MAIN_HANDLE = LoadLibraryW(filepath);

		if (MAIN_HANDLE)
		{
			// ====================================================================================================================== //

			AREA::SaveData = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@@2PEADEA"));

			// ====================================================================================================================== //

			Tz::CMenuHelp::Create = reinterpret_cast<void(*)(char*, bool)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Create@CMenuHelp@Tz@@2P6AXPEAD_N@ZEA"));

			// ====================================================================================================================== //

			Tz::CmTop::GetListBuffer = reinterpret_cast<char** (*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetListBuffer@CmTop@Tz@@2P6APEAPEADXZEA"));
			Tz::CmTop::GetCurPos = reinterpret_cast<int(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetCurPos@CmTop@Tz@@2P6AHH@ZEA"));
			Tz::CmTop::GetSelectPos = reinterpret_cast<int(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSelectPos@CmTop@Tz@@2P6AHH@ZEA"));
			Tz::CmTop::GetTopPlateSeqTbl = reinterpret_cast<char* (*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetTopPlateSeqTbl@CmTop@Tz@@2P6APEADH@ZEA"));

			Tz::CmTop::m_SeqUnit = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_SeqUnit@CmTop@Tz@@2PEAHEA"));
			Tz::CmTop::m_ImgUnit = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_ImgUnit@CmTop@Tz@@2PEAHEA"));
			Tz::CmTop::m_DummySeq = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_DummySeq@CmTop@Tz@@2PEAHEA"));

			Tz::CmTop::m_Lay = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_Lay@CmTop@Tz@@2PEAPEADEA"));
			Tz::CmTop::m_MenuPtr = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_MenuPtr@CmTop@Tz@@2PEAPEADEA"));
			Tz::CmTop::m_ItemInfo = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_ItemInfo@CmTop@Tz@@2PEAPEADEA"));

			// ====================================================================================================================== //

			Tz::MenuBase::GetMode = reinterpret_cast<int(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetMode@MenuBase@Tz@@2P6AHXZEA"));
			Tz::MenuBase::SetSMode = reinterpret_cast<void(*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetSMode@MenuBase@Tz@@2P6AXH@ZEA"));

			// ====================================================================================================================== //

			Tz::PartyInfo::GetName = reinterpret_cast<char* (*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetName@PartyInfo@Tz@@2P6APEADPEADH@ZEA"));
			Tz::PartyInfo::GetSheet = reinterpret_cast<char* (*)(char*, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSheet@PartyInfo@Tz@@2P6APEADPEADH@ZEA"));

			// ====================================================================================================================== //

			Tz::Select::SetItem = reinterpret_cast<void(*)(char*, int, uint32_t, uint32_t, char*, int, uint16_t*, char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetItem@Select@Tz@@2P6AXPEADHII0HPEAG0@ZEA"));
			Tz::Select::GetItemPtr = reinterpret_cast<char* (*)(char*, uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetItemPtr@Select@Tz@@2P6APEADPEAD_K@ZEA"));
			Tz::Select::SetCurrent = reinterpret_cast<void(*)(char*, int, bool, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetCurrent@Select@Tz@@2P6AXPEADH_NHH@ZEA"));
			Tz::Select::SetExOffset = reinterpret_cast<void(*)(char*, int, int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?SetExOffset@Select@Tz@@2P6AXPEADHHH@ZEA"));
			Tz::Select::GetSelectPos = reinterpret_cast<int(*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSelectPos@Select@Tz@@2P6AHPEAD@ZEA"));

			// ====================================================================================================================== //

			Tz::SelHist::Create = reinterpret_cast<void(*)(int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Create@SelHist@Tz@@2P6AXHH@ZEA"));

			// ====================================================================================================================== //

			YS::COMMAND_ELEM::Get = reinterpret_cast<char* (*)(uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Get@COMMAND_ELEM@YS@@2P6APEAD_K@ZEA"));

			// ====================================================================================================================== //

			YS::ITEM_TABLE::Get = reinterpret_cast<char* (*)(uint64_t)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Get@ITEM_TABLE@YS@@2P6APEAD_K@ZEA"));
			YS::ITEM_TABLE::Each = reinterpret_cast<char* (*)(char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?Each@ITEM_TABLE@YS@@2P6APEADPEAD@ZEA"));

			// ====================================================================================================================== //

			YS::MEMBER_TABLE::MemberTable = reinterpret_cast<char*>(*(void**)GetProcAddress(MAIN_HANDLE, "?MemberTable@MEMBER_TABLE@YS@@2PEADEA"));

			// ====================================================================================================================== //

			YS::MESSAGE::GetSize = reinterpret_cast<const size_t(*)(const char*)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetSize@MESSAGE@YS@@2P6A?B_KPEBD@ZEA"));
			YS::MESSAGE::GetData = reinterpret_cast<char* (*)(int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetData@MESSAGE@YS@@2P6APEADH@ZEA"));

			// ====================================================================================================================== //

			Tz::CmCustom::m_pri = reinterpret_cast<int*>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_pri@CmCustom@Tz@@2PEAHEA"));

			Tz::CmCustom::m_ListInfo = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_ListInfo@CmCustom@Tz@@2PEAPEADEA"));
			Tz::CmCustom::m_PartyInfo = reinterpret_cast<char**>(*(void**)GetProcAddress(MAIN_HANDLE, "?m_PartyInfo@CmCustom@Tz@@2PEAPEADEA"));

			Tz::CmCustom::CreateTopList = reinterpret_cast<bool(*)()>(*(void**)GetProcAddress(MAIN_HANDLE, "?CreateTopList@CmCustom@Tz@@2P6AXXZEA"));
			Tz::CmCustom::GetFriendInfo = reinterpret_cast<char*(*)(int, int)>(*(void**)GetProcAddress(MAIN_HANDLE, "?GetFriendInfo@CmCustom@Tz@@2P6APEADHH@ZEA"));

			Tz::CmCustom::CheckKH1Form = reinterpret_cast<bool(*)()>(GetProcAddress(MAIN_HANDLE, "?CheckKH1Form@CmCustom@Tz@@SA_NXZ"));
			Tz::CmCustom::UpdateTopList = reinterpret_cast<bool(*)()>(GetProcAddress(MAIN_HANDLE, "?UpdateTopList@CmCustom@Tz@@SAXXZ"));
			Tz::CmCustom::CurPos2CustomType = reinterpret_cast<int(*)(int)>(GetProcAddress(MAIN_HANDLE, "?CurPos2CustomType@CmCustom@Tz@@SAHH@Z"));

			RedirectFunctionSLIM("?UpdateHelpMess@CmCustom@Tz@@SAXXZ", reinterpret_cast<uint64_t>(UpdateHelpMess));

			// ====================================================================================================================== //
		}
	}

	__declspec(dllexport) void RF_ModuleExecute(wchar_t* mod_path)
	{
		// === IMPORT FROM THE MAIN MODULE === //

		bool* _isMenu = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsMenu@MENU@YS@@2PEA_NEA");
		bool* _isTitle = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsTitle@TITLE@YS@@2PEA_NEA");
		bool* _isInMap = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsInMap@AREA@@2PEA_NEA");
		char* _menuType = *(char**)GetProcAddress(MAIN_HANDLE, "?MenuType@MENU@YS@@2PEADEA");
		char* _subMenuType = *(char**)GetProcAddress(MAIN_HANDLE, "?SubMenuType@MENU@YS@@2PEADEA");
		char* _memberTable = *(char**)GetProcAddress(MAIN_HANDLE, "?MemberTable@MEMBER_TABLE@YS@@2PEADEA");
		char* _saveData = *(char**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@@2PEADEA");

		uint16_t* _hardpadInput = *(uint16_t**)GetProcAddress(MAIN_HANDLE, "?Input@HARDPAD@YS@@2PEAGEA");

		char** _jiminyMenuPtr = *(char***)GetProcAddress(MAIN_HANDLE, "?Jiminy@MENU@YS@@2PEAPEADEA");
		char** _commandMenuPtr = *(char***)GetProcAddress(MAIN_HANDLE, "?CommandMenu@COMMAND_DRAW@dk@@2PEAPEADEA");
		char** _subOptionSelectPtr = *(char***)GetProcAddress(MAIN_HANDLE, "?SubOptionSel@MENU@YS@@2PEAPEADEA");

		using PlaySFX_t = void(*)(uint32_t);
		using GetData_t = const char* (*)(int);
		using GetSize_t = size_t(*)(const char*);
		using DecodeKHSCII_t = string(*)(const char*);
		using EncodeKHSCII_t = vector<char>(*)(string);
		using UpdateListShortcut_t = void(*)(uint32_t);

		PlaySFX_t _playSFX = *(PlaySFX_t*)GetProcAddress(MAIN_HANDLE, "?PlaySFX@SOUND@@2P6AXI@ZEA");
		GetData_t _getData = *(GetData_t*)GetProcAddress(MAIN_HANDLE, "?GetData@MESSAGE@YS@@2P6APEADH@ZEA");
		GetSize_t _getSize = *(GetSize_t*)GetProcAddress(MAIN_HANDLE, "?GetSize@MESSAGE@YS@@2P6A?B_KPEBD@ZEA");
		UpdateListShortcut_t _updateListShortcut = *(UpdateListShortcut_t*)GetProcAddress(MAIN_HANDLE, "?UpdateListShortcut@MENU@YS@@2P6AXI@ZEA");

		DecodeKHSCII_t _decodeKHSCII = (DecodeKHSCII_t)GetProcAddress(MAIN_HANDLE, "?DecodeKHSCII@MESSAGE@YS@@SA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBD@Z");
		EncodeKHSCII_t _encodeKHSCII = (EncodeKHSCII_t)GetProcAddress(MAIN_HANDLE, "?EncodeKHSCII@MESSAGE@YS@@SA?AV?$vector@DV?$allocator@D@std@@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@4@@Z");

		// ==================================== //

		// === BOOLEAN SPACE === //

		bool IS_CUSTOMIZE = *_isMenu && *_menuType == 0x08 && *_subMenuType == 0x19 && !*_jiminyMenuPtr;
		bool IS_SHORTEDIT = *_isMenu && *_menuType == 0x08 && (*_subMenuType == 0x1A || *_subMenuType == 0x1D || *_subMenuType == 0x1E || *_subMenuType == 0x1F) && !*_jiminyMenuPtr;

		bool IS_INPUT_MENU = (*_hardpadInput & 0x0400) == 0x0400 || (*_hardpadInput & 0x0800) == 0x0800;
		bool IS_INPUT_GAME = (*_hardpadInput & 0x0010) == 0x0010 || (*_hardpadInput & 0x0040) == 0x0040;

		bool IS_ROXAS = (*reinterpret_cast<const uint16_t*>(_memberTable) == 0x5A || *reinterpret_cast<const uint16_t*>(_memberTable) == 0x0323);

		bool IS_BLACKLISTED_FORM = *(_saveData + 0x3524) != 0x03 && *(_saveData + 0x3524) != 0x06;

		// ===================== //

		// If we are on the map and the current shortcut set is not what is recorded in the save, set it as such.
		if (!*_isInMap && CURRENT_SHORTCUT_SET != *(_saveData + 0x10104))
		{
			CURRENT_SHORTCUT_SET = *(_saveData + 0x10104);
			memcpy(AREA::SaveData + 0x36F8, AREA::SaveData + 0x10108 + (*(AREA::SaveData + 0x10104) * 0x08), 0x08);
		}

		// If we are not on the title, and the character is not Roxas;
		if (!*_isTitle && !IS_ROXAS)
		{
			// If we are on shortcut editing screen and are not accepting submits; Accept submits.
			if (IS_SHORTEDIT && !SUBMIT_SHORTCUTS)
				SUBMIT_SHORTCUTS = true;

			// If we are not on shortcut editing and are accepting submits;
			else if (!IS_SHORTEDIT && SUBMIT_SHORTCUTS)
			{
				// Submit all shortcuts to the current shortcut set.
				memcpy(_saveData + 0x10108 + (0x08 * CURRENT_SHORTCUT_SET), _saveData + 0x36F8, 0x08);
				SUBMIT_SHORTCUTS = false;
			}

			auto _commandType = *_commandMenuPtr ? reinterpret_cast<uint8_t*>(*_commandMenuPtr) : nullptr;
			auto _subOptionSelect = *_subOptionSelectPtr ? reinterpret_cast<uint8_t*>(*_subOptionSelectPtr) : nullptr;

			// If the command type pointer is not null;
			if (_commandType != nullptr)
			{
				// If we are on any of the related shortcut menus and the input for the menus is nothing, or we are in-game with the shortcuts and the input in-game is nothing; Release the debounce.
				if (((IS_CUSTOMIZE || IS_SHORTEDIT) && !IS_INPUT_MENU) || (*_commandType == 0x05 && !IS_INPUT_GAME))
					DEBOUNCE_SHORTCUT = false;

				// If the debounce is released;
				if (!DEBOUNCE_SHORTCUT)
				{
					// If we are in the in-game shortcuts menu;
					if (*_commandType == 0x05)
					{
						// Fetch the flow direction.
						auto _flowDirection = (*_hardpadInput & 0x0010) == 0x0010 ? -1 : ((*_hardpadInput & 0x0040) == 0x0040 ? 1 : 0);

						// If we are not on Limit or Anti-Form;
						if (IS_BLACKLISTED_FORM)
						{
							// If we have a directional input;
							if (_flowDirection != 0x00)
							{
								// Play the switch sound.
								_playSFX(0x14);

								// Flow the shortcut sets in the given direction and set the debounce.
								CURRENT_SHORTCUT_SET += _flowDirection;
								DEBOUNCE_SHORTCUT = true;
							}
						}

						// If we are and still have input;
						else if (_flowDirection != 0x00)
						{
							// Play the error sound and set the debounce, denying input.
							_playSFX(0x05);
							DEBOUNCE_SHORTCUT = true;
						}
					}

					// If we are on the customize menu specifically, and we have a selection;
					else if (IS_CUSTOMIZE && _subOptionSelect != nullptr)
					{
						auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, 0x00);

						if (_fetchItemPtr)
						{
							if (*reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) == Tz::PartyInfo::GetName(*Tz::CmCustom::m_PartyInfo, 0) && !IS_ROXAS)
								*reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = YS::MESSAGE::GetData(0x5762 + *(AREA::SaveData + 0x10104));
						}

						// If we have the first option selected;
						if (*_subOptionSelect == 0x00)
						{
							// Fetch the flow direction.
							auto _flowDirection = (*_hardpadInput & 0x0400) == 0x0400 ? -1 : ((*_hardpadInput & 0x0800) == 0x0800 ? 1 : 0);

							// If the direction isn't neutral;
							if (_flowDirection != 0x00)
							{
								// Play the switch SFX.
								_playSFX(0x02);

								// Flow the shortcut sets in the flow direction.
								CURRENT_SHORTCUT_SET += _flowDirection;
								DEBOUNCE_SHORTCUT = true;
							}
						}
					}
				}
			}

			// Overflow and underflow protection region. //

			if (CURRENT_SHORTCUT_SET == 0x80)
				CURRENT_SHORTCUT_SET = *(_saveData + 0x10104);

			if (CURRENT_SHORTCUT_SET >= 0x81)
				CURRENT_SHORTCUT_SET = 0x02;

			if (CURRENT_SHORTCUT_SET >= 0x03)
				CURRENT_SHORTCUT_SET = 0x00;

			// ========================================= //

			// If we are in the map and the shortcut set is not equal to the denoted one in the save and we have the debounce set;
			if (*_isInMap && CURRENT_SHORTCUT_SET != *(_saveData + 0x10104) && DEBOUNCE_SHORTCUT)
			{
				// Synchronize the save value.
				memcpy(_saveData + 0x36F8, _saveData + 0x10108 + (0x08 * CURRENT_SHORTCUT_SET), 0x08);
				*(_saveData + 0x10104) = CURRENT_SHORTCUT_SET;

				// Update the shortcut list.
				if (IS_CUSTOMIZE)
				{
					auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, 0x00);

					*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
					*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

					*reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = !IS_ROXAS ? YS::MESSAGE::GetData(0x5762 + *(AREA::SaveData + 0x10104)) : Tz::PartyInfo::GetName(*Tz::CmCustom::m_PartyInfo, 0);

					_updateListShortcut(0x00);
				}
			}
		}
	}
}


