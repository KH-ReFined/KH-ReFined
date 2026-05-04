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

			using AddKnowItemInfo_t = void(*)(char* itemInfo);
			static AddKnowItemInfo_t AddKnowItemInfo;

			using MakeEmptyMsg_t = void(*)(char* itemInfo, int messageId);
			static MakeEmptyMsg_t MakeEmptyMsg;

			using SortItemInfo_t = void(*)(char* itemInfo);
			static SortItemInfo_t SortItemInfo;

			using MakeCommandMsg_t = int(*)(char* itemInfo, int pos, int max);
			static MakeCommandMsg_t MakeCommandMsg;

			using MakeMsgTbl_t = void(*)(char* itemInfo, int pos, int max);
			static MakeMsgTbl_t MakeMsgTbl;

			static char** ItemInfoPtr;
		};
	}
}