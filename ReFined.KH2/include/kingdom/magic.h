#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
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

			using GetTable_t = MAGIC_TABLE * (*)(uint32_t Index);
			static GetTable_t GetTable;

			static char* MagicInfo;
			static  char* MagicCommands;
		};
	}
}