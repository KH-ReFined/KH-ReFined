#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MEMBER
		{
		public:
			using PartToEntryID_t = uint32_t(*)(uint32_t);
			static PartToEntryID_t PartToEntryID;
		};
	}
}