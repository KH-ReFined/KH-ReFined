#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FORM_LEVEL
		{
		public:
			using Search_t = char*(*)(int form, int level);
			static Search_t Search;

			using CheckWisdomAndFinal_t = char*(*)(int part, char* player);
			static CheckWisdomAndFinal_t CheckWisdomAndFinal;

			using GetSummonTable_t = char*(*)();
			static GetSummonTable_t GetSummonTable;
		};
	}
}