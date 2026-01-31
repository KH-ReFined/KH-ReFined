#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <stdint.h>
extern "C"
{
	namespace dk
	{
		class DLL_EXPORT JUMPEFFECT
		{
		public:
			using Out_t = void(*)(uint64_t effectType);
			static Out_t Out;

			static char* FadeStatus;
		};
	}
}