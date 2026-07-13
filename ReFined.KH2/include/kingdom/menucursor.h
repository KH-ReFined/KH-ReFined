#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT MenuCursor
		{
		public:
			static inline void (*SetPri)(int pri, int no) = FindSignature<void (*)(int, int)>("\x44\x8B\xC9\x85\xD2\x79\x34\x33\xD2\x39\x15", "xxxxxxxxxxx");
		};
	}
}