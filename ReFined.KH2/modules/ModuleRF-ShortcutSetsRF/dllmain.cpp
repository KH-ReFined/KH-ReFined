// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <algorithm>

#include <Windows.h>
#include <Psapi.h>
#include <thread>

using namespace std;

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
vector<vector<char>> SHORTCUT_NAMES;

HMODULE MAIN_HANDLE;

struct ModuleInfo
{
	const char* startAddr;
	const char* endAddr;

	ModuleInfo()
	{
		HMODULE hModule = GetModuleHandle(NULL);
		startAddr = reinterpret_cast<const char*>(hModule);

		MODULEINFO info = {};
		GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(info));
		endAddr = startAddr + info.SizeOfImage;
	}
};

static const ModuleInfo moduleInfo;

template <typename T>
T SignatureScan(const char* pattern, const char* mask)
{
	size_t patLen = std::strlen(mask);

	for (const char* addr = moduleInfo.startAddr; addr < moduleInfo.endAddr - patLen; ++addr)
	{
		size_t i = 0;
		for (; i < patLen; ++i)
		{
			if (mask[i] != '?' && pattern[i] != addr[i])
				break;
		}

		if (i == patLen)
			return reinterpret_cast<T>(const_cast<char*>(addr));
	}

	return nullptr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
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

char* OFFSET_INPUT_REG = SignatureScan<char*>("\x40\x53\x55\x41\x54\x41\x55\x41\x56\x48\x83\xEC\x30\x48\x8B\x2D", "xxxxxxxxxxxxxxxx");
vector<uint8_t> INPUT_REG;

extern "C"
{
	__declspec(dllexport) void RF_ModuleInit(const wchar_t* mod_path)
	{
		wchar_t filepath[MAX_PATH];

		wcscpy(filepath, mod_path);
		wcscat(filepath, L"\\dll\\ReFined.KH2.dll");

		MAIN_HANDLE = LoadLibraryW(filepath);
	}

	__declspec(dllexport) void RF_ModuleExecute(wchar_t* mod_path)
	{
		// === IMPORT FROM THE MAIN MODULE === //

		bool* _isMenu  = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsMenu@MENU@YS@@2PEA_NEA");
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
		using GetData_t = const char*(*)(int);
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

		// If the default shortcut name is not parsed;
		if (DEFAULT_SHORTCUT.size() == 0x00)
		{
			// Parse the default shortcut name (String ID: 0x051F)
			DEFAULT_SHORTCUT = _decodeKHSCII(_getData(0x051F));

			// Parse the input registration call for future use.
			INPUT_REG.resize(4);
			memcpy(INPUT_REG.data(), OFFSET_INPUT_REG + 0x109, 0x04);
		}

		// If not on title and the shortcut set names are not parsed;
		if (!*_isTitle && SHORTCUT_NAMES.size() == 0x00)
		{
			// For 3 iterations;
			for (int i = 0; i < 3; i++)
			{
				// Parse the string (From the save data if exists, default if not.)
				auto _parsedString = *(_saveData + 0x20 + (0x20 * i)) != 0x00 ? _decodeKHSCII(_saveData + 0x20 + (0x20 * i)) : DEFAULT_SHORTCUT;

				// If the parsed string is the default one, make the adjustment.
				if (_parsedString == DEFAULT_SHORTCUT)
					replace(_parsedString.begin(), _parsedString.end(), 'X', (char)(65 + i));

				// Push back the name for future use.
				SHORTCUT_NAMES.push_back(_encodeKHSCII(_parsedString));
			}
		}

		// If we are on the title and shortcut names are parsed, clear them.
		else if (*_isTitle && SHORTCUT_NAMES.size() != 0x00)
			SHORTCUT_NAMES.clear();

		// If we are on the map and the current shortcut set is not what is recorded in the save, set it as such.
		if (!*_isInMap && CURRENT_SHORTCUT_SET != *(_saveData + 0x10104))
			CURRENT_SHORTCUT_SET = *(_saveData + 0x10104);

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

			// Fetch the current text pointer and it's size.
			auto _currentTextPtr = _getData(0x051F);
			auto _currentTextSize = _getSize(_currentTextPtr);

			// Fetch the current text as an array.
			vector<char> _currentText(_currentTextSize + 0x01);
			memcpy(_currentText.data(), _currentTextPtr, _currentTextSize + 0x01);

			// If we ain't editing the name of the shortcut set;
			if (!EDITING_SHORTCUT_NAME)
			{
				//If we are not on any related shortcut menu and haven't reverted the name to Sora;
				if (!IS_CUSTOMIZE && !IS_SHORTEDIT && !REVERTED_SHORTNAME)
				{
					// Get the Sora text.
					auto _soraText = _getData(0x572E);
					auto _soraSize = _getSize(_soraText);

					// Apply the Sora text to the current text.
					memcpy(const_cast<char*>(_currentTextPtr), _soraText, _soraSize + 0x01);
					REVERTED_SHORTNAME = true;
				}

				// If we are on any related shortcut menu;
				else if ((IS_CUSTOMIZE || IS_SHORTEDIT))
				{
					// Apply the check between the current text and the shortcut name based on their lengths.
					
					bool _fetchEqual = false;

					if (SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size() > _currentText.size())
						_fetchEqual = equal(_currentText.begin(), _currentText.end(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].begin());

					else
						_fetchEqual = equal(SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].begin(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].end(), _currentText.begin());

					// If the check fails or we reverted the name;
					if (!_fetchEqual || REVERTED_SHORTNAME)
					{
						// Apply the current shortcut set's name to the current text.
						memcpy(const_cast<char*>(_currentTextPtr), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].data(), SHORTCUT_NAMES[CURRENT_SHORTCUT_SET].size());
						REVERTED_SHORTNAME = false;
					}
				}
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
					_updateListShortcut(0x00);
			}
		}
	}
}


