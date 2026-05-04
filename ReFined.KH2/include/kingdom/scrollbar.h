#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT ScrollBar
		{
		public:
			using Create_t = void (*)(char* scrollBar, int pri, char* sqd, char* img, char* seq, int max, int dips, char* parent);
			static Create_t Create;

			using SetPos_t = void (*)(char* scrollBar, int pos);
			static SetPos_t SetPos;
		};
	}
}