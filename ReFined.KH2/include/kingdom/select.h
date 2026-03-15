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

			using SetItem_t = char* (*)(char* select, int pos, int s_unit, int i_unit, char* layout, int message, char* seq_n, char* seq_a);
			static SetItem_t SetItem;
		};
	}
}