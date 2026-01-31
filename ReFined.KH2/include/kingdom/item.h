#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "SigScan.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ITEM
		{
		public:
			using GetNumBackyard_t = uint64_t(*)(uint64_t item);
			static GetNumBackyard_t GetNumBackyard;

			using GetBackyard_t = void(*)(uint16_t item, int num);
			static GetBackyard_t GetBackyard;

			using ReduceBackyard_t = void(*)(uint16_t item, int num);
			static ReduceBackyard_t ReduceBackyard;

			static char* ImageBuff;
			static uint64_t pint_itemmenuinfo;
		};
	}
}