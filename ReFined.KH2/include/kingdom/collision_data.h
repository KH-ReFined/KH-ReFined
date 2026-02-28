#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include <SigScan.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COLLISION_DATA
		{
		public:
			using next_elem_t = char* (*)(char* colldata, int type, char* elem);
			static next_elem_t next_elem;
		};
	}
}