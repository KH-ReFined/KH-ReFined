#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MEMBER_TABLE
		{
		public:
			static char* MemberTable;
			static char* MemberStatsAnchor;
		};
	}
}