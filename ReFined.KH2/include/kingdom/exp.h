#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Exp
		{
		public:
			using getSeqNumber_t = int(*)(char* table, int number, int* outtable);

			static getSeqNumber_t getSeqNumber;
		};
	}
}