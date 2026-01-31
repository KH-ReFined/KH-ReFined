#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT LockOn
		{
		public:
			using getYIImage_t = char*(*)();

			static getYIImage_t getYIImage;
		};
	}
}