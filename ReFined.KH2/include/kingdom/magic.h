#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MAGIC
		{
		public:
			struct DLL_EXPORT MAGIC_TABLE
			{
				uint8_t ID;
				uint8_t Level;
				uint8_t World;
				uint8_t PADDING;
				char Filename[32];
				uint16_t Item;
				uint16_t Command;
				uint16_t Motion;
				uint16_t Blend;
				uint16_t MotionFinish;
				uint16_t BlendFinish;
				uint16_t MotionAir;
				uint16_t BlendAir;
				uint8_t Voice;
				uint8_t VoiceFinish;
				uint8_t VoiceSelf;
				uint8_t VoicePadding;
			};

			static inline MAGIC_TABLE*(*GetTable)(uint32_t Index) = FindSignature<MAGIC_TABLE*(*)(uint32_t )>("\x40\x53\x48\x83\xEC\x20\x4C\x8B\x15\x00\x00\x00\x00\x45\x33\xC9\x41\x8B\xD1", "xxxxxxxxx????xxxxxx");
			static inline bool(*GetMpDriveStatus)(int index) = FindSignature<bool(*)(int)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x63\xC1\x48\x8D\x35", "xxxxxxxxxxxxxxxxxxxx");

			static inline char* MagicInfo = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20\x4C\x8D\x05\x00\x00\x00\x00\xBA\x14\x50\x00\x00\xB9\x01\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxx", 0x41);
			static inline char* MagicCommands = FetchRelativePointer<char*>("\x40\x53\x48\x83\xEC\x20\x8B\x15\x00\x00\x00\x00\x48\x8B\xD9\x85\xD2\x74\x32\x83\xEA\x01\x74\x1F\x83\xEA\x01\x74\x13\x83\xFA\x01\x75\x37\x33\xD2\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xEB\x29\xBA\x02\x00\x00\x00\xEB\x1D\x33\xD2\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xEB\x14\xE8\x00\x00\x00\x00\x33\xD2", "xxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxx????x????xxxxxxxxxxxx????x????xxx????xx", 0x64) + 0xB68;
		};
	}
}