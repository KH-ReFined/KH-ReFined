#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
extern "C"
{
	namespace PC
	{
		class DLL_EXPORT STEAM
		{
		public:

			using CheckSaveFile_t = void(*)();
			static CheckSaveFile_t CheckSaveFile;

			using FetchMareConfig_t = void(*)();
			static FetchMareConfig_t FetchMareConfig;

			static char* MareConfig;
			static uint64_t pint_saveinformation;
		};
	}
}