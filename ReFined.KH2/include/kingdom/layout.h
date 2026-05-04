#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT LAYOUT
		{
		public:
			using GetSequenceUnit_t = char* (*)(char* layout, int no);
			static GetSequenceUnit_t GetSequenceUnit;
		};
	}
}