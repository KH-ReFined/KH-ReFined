#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace kn
	{
		class DLL_EXPORT FriendSaveRam
		{
			public:
				static inline int (*item2index)(int item_id) = FindSignature<int(*)(int)>("\x48\x83\xEC\x28\x45\x33\xC0\x48\x8D\x15\x00\x00\x00\x00\x4C\x8D\x0D", "xxxxxxxxxx????xxx");
		};
	}
}