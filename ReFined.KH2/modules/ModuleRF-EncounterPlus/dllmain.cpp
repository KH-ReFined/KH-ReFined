// dllmain.cpp : Defines the entry point for the DLL application.
#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <vector>
#include <random>

#include <Windows.h>

using namespace std;

HMODULE MAIN_HANDLE;
bool ENCOUNTER_ACTIVE;

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
		char* _saveData = *(char**)GetProcAddress(MAIN_HANDLE, "?SaveData@AREA@YS@@2PEADEA");
		char* _currArea = *(char**)GetProcAddress(MAIN_HANDLE, "?Current@AREA@YS@@2PEAUINFO@12@EA");
		bool* _isInMap = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsInMap@AREA@YS@@2PEA_NEA");
		bool* _isVendor = *(bool**)GetProcAddress(MAIN_HANDLE, "?IsVendor@AREA@YS@@2PEADEA");

		uint64_t _enemyInfoPtr = *(uint64_t*)GetProcAddress(MAIN_HANDLE, "?pint_enemyinfo@AREA@YS@@2_KA");

		uint64_t _roomInfoBase = 0;
		memcpy(&_roomInfoBase, reinterpret_cast<char*>(_enemyInfoPtr), 0x08);

		auto _roomInfo = reinterpret_cast<char*>(_roomInfoBase + 0x08);

		vector<uint16_t> _abilityRead(0x60);
		memcpy(_abilityRead.data(), _saveData + 0x2544, 0xC0);

		bool _moogleGood = *(_saveData + 0x4C34) >= 0x04;

		bool _hasAbility = any_of(_abilityRead.begin(), _abilityRead.end(), [](int x) { return (x == 0x00F8); });
		bool _abilityActive = any_of(_abilityRead.begin(), _abilityRead.end(), [](int x) { return (x == 0x80F8); });

		if (!*_isTitle && *_isInMap && _moogleGood && (!_hasAbility && !_abilityActive))
		{
			auto _indexZero = std::distance(_abilityRead.begin(), std::find(_abilityRead.begin(), _abilityRead.end(), 0x0000));
			*(_saveData + 0x2544 + (_indexZero * 2)) = 0xF8;
		}

		if (!*_isInMap && _abilityActive && !ENCOUNTER_ACTIVE && _roomInfoBase != 0x00)
		{
			char _fillZero[0x100];

			fill(_fillZero, _fillZero + 0x100, 0x00);
			memcpy(const_cast<char*>(_roomInfo), _fillZero, 0x100);

			auto _vendorOffset = *_currArea == 0x05 && *(_currArea + 0x01) == 0x08 ? 0x00 :
				(*_currArea == 0x08 && *(_currArea + 0x01) == 0x02 ? 0x01 :
					(*_currArea == 0x06 && *(_currArea + 0x01) == 0x07 ? 0x02 :
						(*_currArea == 0x07 && *(_currArea + 0x01) == 0x01 ? 0x03 :
							(*_currArea == 0x0E && *(_currArea + 0x01) == 0x06 ? 0x04 : 0xFF))));

			if (_vendorOffset != 0xFF && *reinterpret_cast<char*>(_isVendor + _vendorOffset) != 0x02)
			{
				random_device dev;
				mt19937 rng(dev());
				uniform_int_distribution<mt19937::result_type> randDist(1, 100);

				auto _randomChance = randDist(rng);

				if (_randomChance >= 80)
					*reinterpret_cast<char*>(_isVendor + _vendorOffset) = 0x02;
			}

			ENCOUNTER_ACTIVE = true;
		}

		else if (*_isInMap && ENCOUNTER_ACTIVE)
			ENCOUNTER_ACTIVE = false;
	}
}