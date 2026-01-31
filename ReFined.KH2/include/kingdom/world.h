#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT WORLD
		{
		public:
			using GetName_t = char* (*)(int world);
			static GetName_t GetName;
		};
	}
}