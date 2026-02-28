#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ITEM_TABLE
		{
		public:
			using Each_t = char* (*)(char* itemTable);
			static Each_t Each;

			using Get_t = char* (*)(uint16_t itemId);
			static Get_t Get;
		};
	}
}