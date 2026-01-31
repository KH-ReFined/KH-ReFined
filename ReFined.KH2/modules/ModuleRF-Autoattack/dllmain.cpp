// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <Windows.h>
#include <Psapi.h>
#include <codecvt>

using namespace std;

HMODULE MAIN_HANDLE;

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


inline bool FindModule(const char* moduleName)
{
	HMODULE modules[100];
	void* hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, GetCurrentProcessId());

	if (hProcess)
	{
		DWORD bytesNeeded;
		BOOL rc = EnumProcessModules(hProcess, modules, sizeof(modules), &bytesNeeded);

		if (rc)
		{
			int count = (int)(bytesNeeded / sizeof(HMODULE));

			for (int i = 0; i < count; i++)
			{
				wchar_t fetchName[260];
				GetModuleFileName(modules[i], fetchName, 260);

				wstring_convert<std::codecvt_utf8_utf16<wchar_t>> _converter;

				auto _reqName = _converter.from_bytes(moduleName);
				auto _currName = std::wstring(fetchName);

				if (_currName.find(_reqName) != std::wstring::npos)
					return true;
			}
		}

		return false;
	}
}

extern "C"
{
	__declspec(dllexport) void RF_ModuleInit(const wchar_t* mod_path)
	{
		wchar_t filepath[MAX_PATH];

		wcscpy(filepath, mod_path);
		wcscat(filepath, L"\\dll\\ReFined.KH2.dll");

		MAIN_HANDLE = LoadLibraryW(filepath);
	}

	__declspec(dllexport) void RF_ModuleExecute()
	{
		bool* _isTitle = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsTitle@TITLE@YS@@2PEA_NEA");
		uint8_t* _battleStatus = *(uint8_t**)GetProcAddress(MAIN_HANDLE, "?BattleStatus@AREA@YS@@2PEADEA");
		uint16_t* _hardpadInput = *(uint16_t**)GetProcAddress(MAIN_HANDLE, "?Input@HARDPAD@YS@@2PEAGEA");
		char* _saveData = *(char**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@YS@@2PEADEA");
		char* _memberAnchor = *(char**)GetProcAddress(MAIN_HANDLE, "?MemberStatsAnchor@MEMBER_TABLE@YS@@2PEADEA");
		char* _mareConfig = FindModule("steam_api64.dll") ? *(char**)GetProcAddress(MAIN_HANDLE, "?MareConfig@STEAM@PC@@2PEADEA") : *(char**)GetProcAddress(MAIN_HANDLE, "?MareConfig@EGS@PC@@2PEADEA");

		uint64_t _commandElem = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?CommandElem@COMMAND_ELEM@YS@@2_KA");
		uint64_t _commandDrawPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_commanddraw@COMMAND_DRAW@YS@@2_KA");
		uint64_t _commandMenuPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_commandmenu@COMMAND_DRAW@YS@@2_KA");

		using ExecuteCommand_t = void(*)(char* playerObject, char* command);
		ExecuteCommand_t _executeCommand = *(ExecuteCommand_t*)GetProcAddress(MAIN_HANDLE, "?ExecuteCommand@PARTY@YS@@2P6AXPEAD0@ZEA");

		using CommandElemGet_t = char* (*)(uint64_t id);
		CommandElemGet_t _commandElemGet = *(CommandElemGet_t*)GetProcAddress(MAIN_HANDLE, "?Get@COMMAND_ELEM@YS@@2P6APEAD_K@ZEA");

		using IntToLong_t = uint64_t(*)(uint32_t);
		IntToLong_t _intToLong = *(IntToLong_t*)GetProcAddress(MAIN_HANDLE, "?INT_TO_LONG_ADDRESS@CONVERTER@PC@@2P6A_KI@ZEA");

		uint64_t _playerMainPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_sora@SORA@YS@@2_KA");
		auto _soraPtr = *reinterpret_cast<uint64_t*>(_playerMainPtr);

		auto _pointAction = *reinterpret_cast<uint64_t*>(_commandElem);
		_pointAction = _pointAction + 0x0A;

		auto _currAction = *reinterpret_cast<const uint32_t*>(_pointAction);
		auto _commandPointer = *reinterpret_cast<const char**>(_commandDrawPtr);

		auto _currMainMenu = *reinterpret_cast<const uint64_t*>(_commandMenuPtr);
		auto _currChildMenu = *reinterpret_cast<const uint64_t*>(_commandMenuPtr + 0x08);

		auto _confirmConfig = *(_mareConfig + 0x1E);

		auto _confirmButton = _confirmConfig == 0x00 ? 0x4000 : 0x2000;
		auto _rejectButton = _confirmConfig == 0x01 ? 0x4000 : 0x2000;

		auto _shortcutStart = reinterpret_cast<uint16_t*>(_saveData + 0x36F8);

		if (!*_isTitle && _commandPointer != 0x00)
		{
			auto _currCommand = *reinterpret_cast<const uint8_t*>(*reinterpret_cast<uint64_t*>(_commandMenuPtr) + 0x74);
			auto _mainMenuType = *reinterpret_cast<const uint8_t*>(*reinterpret_cast<uint64_t*>(_commandMenuPtr) + 0x00);
			auto _firstCommand = *reinterpret_cast<const uint16_t*>(*reinterpret_cast<uint64_t*>(_commandMenuPtr) + 0x16);
			auto _commandFlag = *reinterpret_cast<const uint8_t*>(*reinterpret_cast<uint64_t*>(_commandMenuPtr) + 0x19);

			bool _isActionGood = _currChildMenu == 0x00 && _currCommand == 0x00;
			bool _isCommandGood = (_firstCommand == 0x0001 || _firstCommand == 0x0088) && (_commandFlag == 0x00) && (_mainMenuType == 0x00 || _mainMenuType == 0x06);

			auto _fetchButtons = *_hardpadInput;

			if (*_battleStatus != 0x00 && _isCommandGood && _isActionGood && (_fetchButtons & _confirmButton) == _confirmButton && _currAction == 0x00)
			{
				_currAction = 0x01;
				memcpy(reinterpret_cast<char*>(_pointAction), &_currAction, 0x04);
			}

			else if (_mainMenuType == 0x05)
			{
				uint16_t _currCommand = 0x00;

				if (_currAction != 0x00)
				{
					_currAction = 0x00;
					memcpy(reinterpret_cast<char*>(_pointAction), &_currAction, 0x04);
				}

				if (_fetchButtons & 0x4000)
					_currCommand = _confirmButton == 0x01 ? *_shortcutStart : *(_shortcutStart + 0x03); 
				
				else if (_fetchButtons & 0x2000)
					_currCommand = _confirmButton == 0x00 ? *_shortcutStart : *(_shortcutStart + 0x03);

				else if (_fetchButtons & 0x1000)
					_currCommand = *(_shortcutStart + 0x01);

				else if (_fetchButtons & 0x8000)
					_currCommand = *(_shortcutStart + 0x02);

				if (_currCommand != 0x00 && *(_memberAnchor + 0xC308 + 0x180) > 0x00)
				{
					auto _commandElem = _commandElemGet(_currCommand);
					auto _commandAlloc = (char*)malloc(0x20);

					fill(_commandAlloc, _commandAlloc + 0x20, 0x00);

					if (*(_commandElem + 0x1C) > 0x00)
					{
						*reinterpret_cast<uint16_t*>(_commandAlloc) = _currCommand;
						*reinterpret_cast<uint8_t*>(_commandAlloc + 0x05) = *(_commandElem + 0x1C);

						_executeCommand(reinterpret_cast<char*>(_soraPtr), _commandAlloc);

						free(_commandAlloc);
					}
				}
			}

			else if (_currAction == 0x01)
			{
				_currAction = 0x00;
				memcpy(reinterpret_cast<char*>(_pointAction), &_currAction, 0x04);
			}
		}
	}
}