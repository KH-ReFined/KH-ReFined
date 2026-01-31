#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT SEQUENCE
		{
		public:
			using CreateNew_t = void(*)(char* SEQUENCE);
			using Init_t = void(*)(char* SEQUENCE, char* seqd, char* image);
			using SetNumber_t = void(*)(char* SEQUENCE, int no);
			using SetNumberForce_t = void(*)(char* SEQUENCE, int no);
			using GetParamCr_t = uint64_t(*)(char* SEQUENCE);
			using GetParamEx_t = uint64_t(*)(char* SEQUENCE);

			static CreateNew_t CreateNew;
			static Init_t Init;
			static SetNumber_t SetNumber;
			static SetNumberForce_t SetNumberForce;
			static GetParamCr_t GetParamCr;
			static GetParamEx_t GetParamEx;
		};
	}
}