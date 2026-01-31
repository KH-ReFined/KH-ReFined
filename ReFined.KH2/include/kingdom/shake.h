#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace ryj
	{
		class DLL_EXPORT SHAKE
		{
		public:
			static float* ShakeTimer;
			static short* ShakeCoords;
		};
	}
}