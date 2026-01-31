#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT WEAPON_MSET
		{
		public:
			using GetFilename_t = char* (*)(int part, int hand, char* buff);
			static GetFilename_t GetFilename;
		};
	}
}