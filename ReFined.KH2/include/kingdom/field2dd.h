#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Field2DD
		{
		public:
			using getSqd_t = char* (*)(int num);

			static getSqd_t getSqd;
		};
	}
}