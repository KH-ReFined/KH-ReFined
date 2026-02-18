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
			using ReadCallback = void(*)(uint32_t*, size_t, uint32_t*);

			using GetSize_t = size_t(*)(const char* fileName);
			static GetSize_t GetSize;

			using LoadBAR_t = size_t(*)(const char* fileName, char* address);
			static LoadBAR_t LoadBAR;

			using Read_t = size_t(*)(const char* fileName, char* address);
			static Read_t Read;

			using ReadBack_t = void(*)(const char* fileName, char* address, ReadCallback callback, uint32_t arguments);
			static ReadBack_t ReadBack;

			using ReadBarBack_t = char*(*)(const char* fileName, char* address, ReadCallback callback, uint32_t* arguments);
			static ReadBarBack_t ReadBarBack;
		};
	}
}