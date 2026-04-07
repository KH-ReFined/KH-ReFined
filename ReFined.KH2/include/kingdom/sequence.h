#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT SEQUENCE
		{
		public:
			using SEQUENCE_t = void(*)(char* SEQUENCE);
			using Dispose_t = void(*)(char* SEQUENCE);
			using Init_t = void(*)(char* SEQUENCE, char* seqd, char* image);
			using SetNumber_t = void(*)(char* SEQUENCE, int no);
			using Control_t = void(*)(char* SEQUENCE);
			using Draw_t = void(*)(char* SEQUENCE);
			using SetNumberForce_t = void(*)(char* SEQUENCE, int no);
			using GetParamCr_t = uint64_t(*)(char* SEQUENCE);
			using GetParamEx_t = uint64_t(*)(char* SEQUENCE);
			using GetParamSize_t = uint64_t(*)(char* SEQUENCE);
			using GetParamX_t = uint64_t(*)(char* SEQUENCE);
			using GetParamY_t = uint64_t(*)(char* SEQUENCE);
			using GetActiveX_t = uint32_t(*)(char* SEQUENCE);
			using GetActiveY_t = uint32_t(*)(char* SEQUENCE);
			using SetScaleX_t = void(*)(char* SEQUENCE, float scaleX);
			using GetActiveRGBA_t = uint32_t(*)(char* SEQUENCE);

			static SEQUENCE_t _SEQUENCE;
			static Dispose_t Dispose;
			static Init_t Init;
			static SetNumber_t SetNumber;
			static Control_t Control;
			static Draw_t Draw;
			static SetNumberForce_t SetNumberForce;
			static GetParamCr_t GetParamCr;
			static GetParamEx_t GetParamEx;
			static GetParamSize_t GetParamSize;
			static GetParamX_t GetParamX;
			static GetParamY_t GetParamY;
			static GetActiveX_t GetActiveX;
			static GetActiveY_t GetActiveY;
			static SetScaleX_t SetScaleX;
			static GetActiveRGBA_t GetActiveRGBA;
		};
	}
}