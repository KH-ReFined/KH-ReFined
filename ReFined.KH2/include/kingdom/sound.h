#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
#include "SigScan.h"
extern "C"
{
	namespace YS
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
		};
	}
}