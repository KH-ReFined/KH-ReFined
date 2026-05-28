#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT FILE
		{
		public:
			using ReadCallback = void(*)(uint32_t*, size_t, uint32_t*);

			static inline size_t(*GetSize)(const char* fileName) = FindSignature<size_t(*)(const char*)>("\x40\x53\x48\x81\xEC\x30\x01\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x20\x01\x00\x00\x48\x8D\x15\x00\x00\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxxxxx????");
			static inline size_t(*ReadBAR)(const char* fileName, char* address) = FindSignature<size_t(*)(const char*, char*)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xFA\xE8\x00\x00\x00\x00\x48\x63\xD8\x85\xC0\x74\x08\x48\x8B\xCF\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxx????xxxxxxxxxxx????");
			static inline size_t(*Read)(const char* fileName, char* address) = FindSignature<size_t(*)(const char*, char*)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x81\xEC\x30\x01", "xxxxxxxxxxxxxxxx");
			static inline void(*ReadBack)(const char* fileName, char* address, ReadCallback callback, uint32_t arguments) = FindSignature<void(*)(const char*, char*, ReadCallback, uint32_t)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x49\x8B\xD9\x49\x8B\xF8\x48\x8B\xF2\x48\x8B\xE9\xE8\x00\x00\x00\x00\x84\xC0\x75\x32", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx");
			static inline char*(*ReadBarBack)(const char* fileName, char* address, ReadCallback callback, uint32_t* arguments) = FindSignature<char* (*)(const char*, char*, ReadCallback, uint32_t*)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x49\x8B\xD9\x49\x8B\xF8\x48\x8B\xF2\x48\x8B\xE9\xE8\x00\x00\x00\x00\x84\xC0\x75\x35", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxx");
			static inline bool(*IsReading)() = FetchFunctionFromCall<bool(*)()>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x48\x83\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x13", "xxxxxxxxxxxxx????xxxxxxxxxx????xxxxxx", 0xB1);
		};
	}
}