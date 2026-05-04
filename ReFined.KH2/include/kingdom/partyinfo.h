#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

#include <thread>
#include <cassert>
#include <member_table.h>

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT PartyInfo
		{
		public:
			using GetSheet_t = char*(*)(char* partyInfo, int num);
			static GetSheet_t GetSheet;

			using GetName_t = char*(*)(char* partyInfo, int num);
			static GetName_t GetName;
		};
	}
}