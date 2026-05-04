#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace kn
	{
		class DLL_EXPORT FriendSaveRam
		{
			public:
				static int (*item2index)(int item_id);
		};
	}
}