#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ITEM_TABLE
		{
		public:
			static char* (*Each)(char* itemTable);
			static char* (*Get)(uint64_t itemId);
		};
	}
}