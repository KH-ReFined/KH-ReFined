#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ITEM
		{
		public:
			static char** WeaponEntry;

			using GetNum_t = uint64_t(*)(uint64_t item, uint64_t part);
			static GetNum_t GetNum;

			using GetNumBackyard_t = uint64_t(*)(uint64_t item);
			static GetNumBackyard_t GetNumBackyard;

			using GetBackyard_t = void(*)(int item, int num);
			static GetBackyard_t GetBackyard;

			using ReduceBackyard_t = void(*)(uint64_t item, int num);
			static ReduceBackyard_t ReduceBackyard;

			using GetCommand_t = int(*)(uint64_t item);
			static GetCommand_t GetCommand;

			static char* ImageBuff;
		};
	}
}