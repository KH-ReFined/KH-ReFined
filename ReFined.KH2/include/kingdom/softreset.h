#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace dk
	{
		class DLL_EXPORT SOFTRESET
		{
		public:
			using SoftResetThread_t = void(*)();
			static SoftResetThread_t SoftResetThread;

			static bool* RESET;
		};
	}

}