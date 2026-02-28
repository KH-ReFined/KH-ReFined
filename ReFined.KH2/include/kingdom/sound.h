#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "memorymgr.h"

#include "area.h"
#include "region.h"
#include "file.h"

extern "C"
{
	class DLL_EXPORT SOUND
	{
	public:
		static uint64_t* IsTransferActive;
		static char* CurrentMusic;

		using BGMFadeOut_t = void(*)(uint32_t fadeTime, uint32_t mode);
		static BGMFadeOut_t BGMFadeOut;

		using KillBGM_t = void(*)(uint32_t mode);
		static KillBGM_t KillBGM;

		using StartBGM_t = void(*)(uint32_t mode, uint32_t start_vol, uint32_t end_vol, uint32_t count);
		static StartBGM_t StartBGM;

		using PlaySFX_t = void(*)(uint32_t soundID);
		static PlaySFX_t PlaySFX;

		using StreamAllStop_t = void(*)(bool flag);
		static StreamAllStop_t StreamAllStop;

		using SetTransfer_t = void(*)(int bank, int type, char* scdFile, size_t scdSize, char* callback, char* args);
		static SetTransfer_t SetTransfer;

		using PlayVSB_t = void(*)(char* vsbData, size_t vsbSize, int volume, int pan);
		static PlayVSB_t PlayVSB;

		static char* BGM_WRITE_BUFFER;
		static char* getBgmFileName(int number);

		struct staticInitializer
		{
			staticInitializer()
			{
				printf("======================================================\n");
				printf("Handling hooks and redirections concerning SOUND...\n\n");

				vector<uint8_t> _absoluteInstructionJMP =
				{
					0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
				};

				auto getBgmFileName_orig = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x80\x3D\x00\x00\x00\x00\x0D\x8B\xD9\x75\x00\x83\xF9\x75\xBB\x05\x02\x00\x00\xB8\x09\x02\x00\x00\x0F\x45", "xxxxxxxx????xxxx?xxxxxxxxxxxxxxx");
				uint64_t _constFunction = reinterpret_cast<uint64_t>(SOUND::getBgmFileName);

				printf("Fetched SOUND::getBgmFileName @ 0x%p\n", getBgmFileName_orig);

				fill(getBgmFileName_orig, getBgmFileName_orig + 0x78, 0x90);

				memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
				memcpy(getBgmFileName_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

				printf("Hooked SOUND::getBgmFileName [0x%p] to Re:Fined function @ 0x%p\n", getBgmFileName_orig, SOUND::getBgmFileName);

				printf("\nSuccessfully handled SOUND concerns.\n");
				printf("======================================================\n\n");
			}
		};

        static staticInitializer initialize;
	};
}