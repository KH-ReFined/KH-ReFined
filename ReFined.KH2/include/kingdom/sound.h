#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "area.h"
#include "file.h"
#include "region.h"
#include "memorymgr.h"

extern "C"
{
	class DLL_EXPORT SOUND
	{
	private:
		static bool _init()
		{
			RedirectFunction("\x40\x53\x48\x83\xEC\x20\x80\x3D\x00\x00\x00\x00\x0D\x8B\xD9\x75\x00\x83\xF9\x75\xBB\x05\x02\x00\x00\xB8\x09\x02\x00\x00\x0F\x45", "xxxxxxxx????xxxx?xxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(getBgmFileName), 0x78);
			return true;
		}

		#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
		static inline bool _doInit = _init();
		#endif

	public:
		static inline uint64_t* IS_TRANSFER_ACTIVE = FetchRelativePointer<uint64_t*>("\x40\x53\x48\x83\xEC\x20\x8B\x05\x00\x00\x00\x00\x4C\x8D\x1D\x00\x00\x00\x00\xFF\xC8\x89\x05\x00\x00\x00\x00\x4C\x63\xD0\x4B\x63\x84\x93\x00\x09\x00\x00\x48\x8D\x1C\x40\x48\x8B\x44\x24\x50", "xxxxxxxx????xxx????xxxx????xxxxxxxxxxxxxxxxxxxx", 0x66);
		static inline char* CURRENT_MUSIC = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x56\x57\x41\x56\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x0F\xB7\x10\x0F\xB7\x48\x02\x83\x25\x00\x00\x00\x00\xFC\x83\x25\x00\x00\x00\x00\xFC", "xxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxx????x", 0x68);

		static inline void(*BGMFadeOut)(uint32_t fadeTime, uint32_t mode) = FetchFunctionFromCall<void(*)(uint32_t, uint32_t) >("\x48\x83\xEC\x38\x80\x3D\x00\x00\x00\x00\x00\x0F\x84\xD7\x00\x00\x00\x48\x89\x5C\x24\x30", "xxxxxx????xxxxxxxxxxxx", 0xAA);
		static inline void(*KillBGM)(uint32_t mode) = FindSignature<void(*)(uint32_t)>("\x40\x53\x48\x83\xEC\x20\x48\x83\x3D\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x48\x8B\x1D\x00\x00\x00\x00", "xxxxxxxxx????xxx????xxx????");
		static inline void(*StartBGM)(uint32_t mode, uint32_t start_vol, uint32_t end_vol, uint32_t count) = FindSignature<void(*)(uint32_t, uint32_t, uint32_t, uint32_t)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x30\x8B\x7C\x24\x60\x41\x8B\xF1\x41\x8B\xE8\x44\x8B\xF2\x8B\xD9\x85\xFF\x75\x05\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
		static inline void(*PlaySFX)(uint32_t soundID) = FindSignature<void(*)(uint32_t)>("\x48\x83\xEC\x00\x44\x8B\xC2\xC7\x44\x24\x20\x00\x00\x00\x00", "xxx?xxxxxxx????");

		static inline void(*StreamAllStop)(bool flag) = FindSignature<void(*)(bool)>("\x40\x56\x48\x83\xEC\x20\x48\x83\x3D\x00\x00\x00\x00\x00\x8B\xF1\x0F\x84\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\x8B\x50\x48\x48\x89\x5C\x24\x30\x48\x89\x6C\x24\x38\x33\xED\x48\x85\xD2\x48\x89\x7C\x24\x40\x48\x8D\x5A\xC0\x48\x0F\x44\xDD\x85\xF6\x74\x09\x83\xB8\x7C\x01\x00\x00\x02", "xxxxxxxxx????xxxxx????xxx????xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
		static inline void(*SetTransfer)(int bank, int type, char* scdFile, size_t scdSize, char* callback, char* args) = FindSignature<void(*)(int, int, char*, size_t, char*, char*)>("\x40\x53\x48\x83\xEC\x20\x8B\x05\x00\x00\x00\x00\x4C\x8D\x1D\x00\x00\x00\x00\xFF\xC8\x89\x05\x00\x00\x00\x00\x4C\x63\xD0\x4B\x63\x84\x93\x00\x09\x00\x00\x48\x8D\x1C\x40\x48\x8B\x44\x24\x50", "xxxxxxxx????xxx????xxxx????xxxxxxxxxxxxxxxxxxxx");
		static inline void(*PlayVSB)(char* vsbData, size_t vsbSize, int volume, int pan) = FindSignature<void(*)(char*, size_t, int, int)>("\x48\x83\xEC\x28\x0F\x57\xD2\x45\x85\xC0\x74\x10\x66\x41\x0F\x6E\xD0\x0F\x5B\xD2\xF3\x0F\x59\x15\x00\x00\x00\x00\x45\x33\xC0\x4C\x8D\x0D\x00\x00\x00\x00\x66\x66\x0F\x1F\x84\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxx????xxxxxx????xxxxxxxxxx");

		static inline char* BGM_WRITE_BUFFER = nullptr;

		static char* getBgmFileName(int number)
		{
			auto _calcNumber = number;

			if (BGM_WRITE_BUFFER == nullptr)
				BGM_WRITE_BUFFER = (char*)malloc(0x28);

			if (AREA::Current->World == 0x0B)
			{
				_calcNumber = 517;

				if (number != 117)
					_calcNumber = number;

				if (number == 121)
					_calcNumber = 521;
			}

			if (YS::REGION::Get() && YS::REGION::Get() != 0x07 && (_calcNumber <= 3 || _calcNumber == 113))
				_calcNumber += 400;

			auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);
			auto _fetchMusic = (_fetchConfig & 0x0080) == 0x0080 ? 0x0080 : ((_fetchConfig & 0x0100) == 0x0100 ? 0x0100 : 0x0000);

			string _constructPath = _fetchConfig & 0x0080 ? "bgm_2nd/music%03d.win32.scd" : (_fetchConfig & 0x0100 ? "bgm_3rd/music%03d.win32.scd" : "bgm/music%03d.win32.scd");

			sprintf_s(BGM_WRITE_BUFFER, 0x28, _constructPath.c_str(), _calcNumber);

			if (YS::FILE::GetSize(BGM_WRITE_BUFFER) == 0x00)
				sprintf_s(BGM_WRITE_BUFFER, 0x28, "bgm/music%03d.win32.scd", _calcNumber);

			return BGM_WRITE_BUFFER;
		}
	};
}