#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT PartyInfo
		{
		public:
			static inline char*(*GetSheet)(char* partyInfo, int num) = FindSignature<char* (*)(char*, int) >("\x48\x63\xC2\x48\xFF\xC0\x48\xC1\xE0\x05\x48\x8B\x04\x08\xC3", "xxxxxxxxxxxxxxx"); 
			static inline char*(*GetName)(char* partyInfo, int num) = FindSignature<char* (*)(char*, int)>("\x48\x63\xC2\x48\xC1\xE0\x05\x48\x8B\x44\x08\x10\xC3", "xxxxxxxxxxxxx");
		};
	}
}