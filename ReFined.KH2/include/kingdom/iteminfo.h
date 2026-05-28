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
			static inline void(*SetItemInfo)(char* itemInfo, int type, int item_type) = FindSignature<void(*)(char*, int, int)>("\x40\x53\x56\x57\x41\x54\x41\x56\x48\x83\xEC\x20\x4C\x8D\x71\x04", "xxxxxxxxxxxxxxxx");
			static inline void(*AddKnowItemInfo)(char* itemInfo) = FindSignature<void(*)(char*)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\xBB\xE8", "xxxxxxxxxxxxxxx");
			static inline void(*MakeEmptyMsg)(char* itemInfo, int messageId) = FindSignature<void(*)(char*, int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xD9\x85\xD2\x0F\x88\x98\x00\x00\x00\x75\x54", "xxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*SortItemInfo)(char* itemInfo) = FindSignature<void(*)(char*)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x89\x7C\x24\x38\x33\xC9\x33\xFF\xE8", "xxxxxxxxxxxxxxxxxxx");
			static inline int(*MakeCommandMsg)(char* itemInfo, int pos, int max) = FindSignature<int(*)(char*, int, int)>("\x45\x85\xC0\x0F\x8E\x78\x01\x00\x00\x89\x54\x24\x10\x57\x41\x55", "xxxxxxxxxxxxxxxx");
			static inline void(*MakeMsgTbl)(char* itemInfo, int pos, int max) = FindSignature<void(*)(char*, int, int)>("\x40\x53\x55\x56\x57\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20\x48", "xxxxxxxxxxxxxxxx");

			static inline char** ItemInfoPtr = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x80\x3D\x00\x00\x00\x00\x00\xBE\xFF\xFF\xFF\xFF\x75\x10\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xF8\xEB\x02\x8B\xFE\xE8\x00\x00\x00\x00\x8B\xD8", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxx????x????xxxxxxx????xx", 0x343) - 0x10;
		};
	}
}