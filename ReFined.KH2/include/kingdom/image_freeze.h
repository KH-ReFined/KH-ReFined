#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT IMAGE_FREEZE
		{
		public:
			using GetImage_t = char* (*)(char* imageFreeze, int no);
			static GetImage_t GetImage;
		};
	}
}