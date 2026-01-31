#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <Windows.h>

using namespace std;

uint8_t ROXAS_SKIP_STAGE;
bool SKIPPING_ROXAS;

HMODULE MAIN_HANDLE;

BOOL APIENTRY DllMain(HMODULE hModule,
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

extern "C"
{
	__declspec(dllexport) bool* INTRO_SEEK;

	__declspec(dllexport) uint32_t* RF_CheckIntro()
	{
		uint32_t _tempInt[] = { 0x02, 0x5730, 0xFFFF, 0x5738, 0x5739, 0x5731, 0x5732, 0xFFFF, 0xFFFF };
		char* _allocMemory = (char*)malloc(0x09 * 0x04);

		memcpy(_allocMemory, _tempInt, 0x09 * 0x04);
		return reinterpret_cast<uint32_t*>(_allocMemory);
	}

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
		char* _saveData = *(char**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@YS@@2PEADEA");
		bool* _isInMap = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsInMap@AREA@YS@@2PEA_NEA");
		bool* _isVendor = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsVendor@AREA@YS@@2PEADEA");
		char* _introSelect = *(char**)GetProcAddress(MAIN_HANDLE, "?IntroSelect@TITLE@YS@@2PEAEEA");
		char* _currArea = *(char**)GetProcAddress(MAIN_HANDLE, "?Current@AREA@YS@@2PEAUINFO@12@EA");
		char* _memberStatsAnchor = *(char**)GetProcAddress(MAIN_HANDLE, "?MemberStatsAnchor@MEMBER_TABLE@YS@@2PEADEA");

		uint64_t _eventInfoPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_eventinfo@EVENT@YS@@2_KA");

		using MapJump_t = void (*)(char*, uint32_t, uint32_t, bool);
		MapJump_t _mapJump = *(MapJump_t*)GetProcAddress(MAIN_HANDLE, "?MapJump@AREA@YS@@2P6AXPEAUINFO@12@II_N@ZEA");

		using GetFileSize_t = size_t (*)(const char*);
		GetFileSize_t _getFileSize = *(GetFileSize_t*)GetProcAddress(MAIN_HANDLE, "?GetSize@FILE@YS@@2P6A_KPEBD@ZEA");

		using LoadBAR_t = size_t (*)(const char*, char*);
		LoadBAR_t _barLoad = *(LoadBAR_t*)GetProcAddress(MAIN_HANDLE, "?LoadBAR@FILE@YS@@2P6A_KPEBDPEAD@ZEA");

		if (*_isTitle)
		{
			if (*_introSelect != 0x00 && ROXAS_SKIP_STAGE == 0x00)
				ROXAS_SKIP_STAGE = 0x02;

			else if (*_introSelect == 0x00 && ROXAS_SKIP_STAGE != 0x00)
				ROXAS_SKIP_STAGE = 0x00;
		}

		if (!*_isTitle)
		{
			uint64_t _eventPointer = *reinterpret_cast<uint64_t*>(_eventInfoPtr);
			_eventPointer = _eventPointer == 0x00 ? 0x00 : *reinterpret_cast<uint64_t*>(_eventPointer + 0x04);

			if (*_currArea == 0x02 && *(_currArea + 0x01) == 0x01 && *(_currArea + 0x04) == 0x38 && ROXAS_SKIP_STAGE == 0x00)
			{
				if (*INTRO_SEEK)
				{
					char _areaArr[] = { 0x02, 0x20, 0x32, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

					uint32_t _writeFlag = 0x00;
					memcpy(_saveData + 0x1CD4, &_writeFlag, 0x04);

					_writeFlag = 0x1FF00001;
					memcpy(_saveData + 0x1CD0, &_writeFlag, 0x04);

					_mapJump(_areaArr, 0x01, 0, false);
					ROXAS_SKIP_STAGE = 0x01;
				}

				else
					ROXAS_SKIP_STAGE = 0x02;
			}

			if (*_currArea == 0x02 && *(_currArea + 0x01) == 0x20 && *(_currArea + 0x04) == 0x01 && ROXAS_SKIP_STAGE == 0x01 && _eventPointer != 0x00)
			{
				size_t _fetchSize = _getFileSize("00prologue.bin");
				auto _loadBAR = (char*)malloc(_fetchSize);

				uint8_t _fetchWeapon = *reinterpret_cast<const uint8_t*>(_saveData + 0x24FE);
				uint32_t _fetchConfig = *reinterpret_cast<const uint32_t*>(_saveData + 0x41A4);

				uint8_t _fetchDifficulty = *reinterpret_cast<const uint8_t*>(_saveData + 0x2498);

				if (_barLoad("00prologue.bin", _loadBAR) != 0x00)
				{
					memcpy(_saveData + 0x031C, _loadBAR, 0x4D);
					memcpy(_saveData + 0x03E8, _loadBAR + 0x4D, 0x01);
					memcpy(_saveData + 0x03EE, _loadBAR + 0x4E, 0x01);
					memcpy(_saveData + 0x1CE2, _loadBAR + 0x4F, 0x01);
					memcpy(_saveData + 0x1CD0, _loadBAR + 0x70, 0x20);
					memcpy(_saveData + 0x20E4, _loadBAR + 0x90, _fetchSize - 0x80);

					if (_fetchDifficulty == 0x03)
					{
						memcpy(_saveData + 0x2544, _loadBAR + 0x50, 0x14);
						memcpy(_saveData + 0x24F8, _loadBAR + 0x66, 0x01);

						memcpy(_memberStatsAnchor + 0xC30C, _loadBAR + 0x64, 0x01);
					}

					else
					{
						memcpy(_saveData + 0x2544, _loadBAR + 0x5E, 0x06);
						memcpy(_memberStatsAnchor + 0xC30C, _loadBAR + 0x68, 0x01);
					}

					memcpy(_saveData + 0x24FE, &_fetchWeapon, 0x01);
					memcpy(_saveData + 0x2498, &_fetchDifficulty, 0x01);

					memcpy(_saveData + 0x41A4, &_fetchConfig, 0x06);
					memcpy(_saveData + 0x4270, _loadBAR + 0x6E, 0x02);

					*(_currArea + 0x01) = 0x0E;
					*(_currArea + 0x04) = 0x02;
					*(_currArea + 0x08) = 0x12;

					_mapJump(_currArea, 0x01, 0, false);
					ROXAS_SKIP_STAGE = 0x02;

					free(_loadBAR);
				}
			}
		}
	}
}