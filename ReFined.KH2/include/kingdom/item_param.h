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
			using Get_t = uint16_t* (*)(char* itemTable);
			static Get_t Get;
		};
	}
}