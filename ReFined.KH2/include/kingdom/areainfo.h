#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT AREAINFO
		{
		public:
			using Get_t = char*(*)(int area, int world);
			static Get_t Get;
		};
	}
}