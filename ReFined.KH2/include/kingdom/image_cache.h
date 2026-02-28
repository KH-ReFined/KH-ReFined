#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include <SigScan.h>

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT IMAGE_CACHE
		{
		public:
			using Load_t = char*(*)(char* imageBuff);
			static Load_t Load;
		};
	}
}