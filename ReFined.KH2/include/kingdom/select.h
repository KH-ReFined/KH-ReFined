#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT Select
		{
		public:
			using SetCurrent_t = void(*)(char* select, int pos, bool isSet, int num, int next);
			static SetCurrent_t SetCurrent;

			using SetItem_t = void (*)(char* select, int pos, uint32_t s_unit, uint32_t i_unit, char* layout, int message, uint16_t* seq_n, char* seq_a);
			static SetItem_t SetItem;

			using GetItemPtr_t = char* (*)(char* select, uint64_t pos);
			static GetItemPtr_t GetItemPtr;

			using GetSelectPos_t = int (*)(char* select);
			static GetSelectPos_t GetSelectPos;

			using SetExOffset_t = void (*)(char* select, int pos, int x, int y);
			static SetExOffset_t SetExOffset;

			using SetPageMode_t = void (*)(char* select, int mode);
			static SetPageMode_t SetPageMode;
			using SetSelectMax_t = void (*)(char* select, int max);
			static SetSelectMax_t SetSelectMax;
		};
	}
}