#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FORM_LEVEL
		{
		public:
			static inline char*(*Search)(int form, int level) = FindSignature<char* (*)(int, int)>("\x48\x83\xEC\x38\x48\x63\xC2\x41\xB9\x08\x00\x00\x00", "xxxxxxxxxxxxx");
			static inline char*(*CheckWisdomAndFinal)(int part, char* player) = FindSignature<char* (*)(int, char*)>("\x40\x53\x48\x83\xEC\x20\x8B\x82\xE0\x0D\x00\x00\x48\x8B\xDA\x83", "xxxxxxxxxxxxxxxx");
			static inline char*(*GetSummonTable)() = reinterpret_cast<char* (*)()>(FindSignature<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x8B\x15\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x33\xDB\x48", "xxxxxxxxxxxxxxxxxx????xxx????xxx") + 0xC0);
		};
	}
}