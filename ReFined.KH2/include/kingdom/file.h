#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FILE
		{
		public:
			using GetSize_t = size_t(*)(const char* fileName);
			static GetSize_t GetSize;

			using LoadBAR_t = size_t(*)(const char* fileName, char* address);
			static LoadBAR_t LoadBAR;

			using Read_t = size_t(*)(const char* fileName, char* address);
			static Read_t Read;
		};
	}
}