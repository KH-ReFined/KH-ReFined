#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT ItemInfo
		{
		public:
			using SetItemInfo_t = void(*)(char* itemInfo, int type, int item_type);
			static SetItemInfo_t SetItemInfo;

			using MakeMsgTbl_t = void(*)(char* itemInfo, int pos, int max);
			static MakeMsgTbl_t MakeMsgTbl;

			static char** ItemInfoPtr;
		};
	}
}