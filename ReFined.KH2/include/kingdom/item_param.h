#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C" 
{
	namespace YS
	{
		class DLL_EXPORT ITEM_PARAM
		{
		public:
			static inline uint16_t* (*Get)(char* itemTable) = FindSignature<uint16_t*(*)(char*)>("\x0F\xB6\x41\x02\x3C\x02\x72\x1A\x3C\x0D\x76\x0D\x3C\x0F\x77\x12\x0F\xB7\x49\x04\xE9\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????");
		};
	}
}