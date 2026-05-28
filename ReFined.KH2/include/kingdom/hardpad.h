#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT HARDPAD
		{
		public:
			static inline void(*Init)() = FindSignature<void(*)()>("\x48\x83\xEC\x28\xF3\x0F\x10\x05\x00\x00\x00\x00\xF3\x0F\x10\x0D\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\xF3\x0F\x11\x05\x00\x00\x00\x00\xF3\x0F\x10\x05\x00\x00\x00\x00\xF3\x0F\x11\x05\x00\x00\x00\x00\xF3\x0F\x11\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x66\x0F\x6F\x0D\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x33\xD2", "xxxxxxxx????xxxx????xxx????xxxx????xxxx????xxxx????xxxx????x????xxxx????xxx????xx");
			static inline uint16_t* Input = FetchRelativePointer<uint16_t*>("\x48\x83\xEC\x28\xF3\x0F\x10\x05\x00\x00\x00\x00\xF3\x0F\x10\x0D\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\xF3\x0F\x11\x05\x00\x00\x00\x00\xF3\x0F\x10\x05\x00\x00\x00\x00\xF3\x0F\x11\x05\x00\x00\x00\x00\xF3\x0F\x11\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x66\x0F\x6F\x0D\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x33\xD2", "xxxxxxxx????xxxx????xxx????xxxx????xxxx????xxxx????xxxx????x????xxxx????xxx????xx", 0x4B);
			
			enum DLL_EXPORT BUTTONS : uint16_t
			{
				NONE = 0x0000,
				SELECT = 0x0001,
				L3 = 0x0002,
				R3 = 0x0004,
				START = 0x0008,
				UP = 0x0010,
				RIGHT = 0x0020,
				DOWN = 0x0040,
				LEFT = 0x0080,
				L2 = 0x0100,
				R2 = 0x0200,
				L1 = 0x0400,
				R1 = 0x0800,
				TRIANGLE = 0x1000,
				CIRCLE = 0x2000,
				CROSS = 0x4000,
				SQUARE = 0x8000,
			};

			static inline map<string, uint16_t> BUTTONS_MAP =
			{
				{ "SELECT",  0x0001 },
				{ "L3",  0x0002 },
				{ "R3",  0x0004 },
				{ "START",  0x0008 },
				{ "UP",  0x0010 },
				{ "RIGHT",  0x0020 },
				{ "DOWN",  0x0040 },
				{ "LEFT",  0x0080 },
				{ "L2",  0x0100 },
				{ "R2",  0x0200 },
				{ "L1",  0x0400 },
				{ "R1",  0x0800 },
				{ "TRIANGLE",  0x1000 },
				{ "CIRCLE",  0x2000 },
				{ "CROSS",  0x4000 },
				{ "SQUARE",  0x8000 },
			};
		};
	}
}