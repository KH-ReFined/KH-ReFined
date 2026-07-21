#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "field.h"
#include "memorymgr.h"

#include <thread>
#include <future>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT CACHE_BUFF
		{ 
			public:
				static inline bool(*IsFlushing)() = FindSignature<bool(*)()>("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x84\xC0\x75\x3C\xE8\x00\x00\x00\x00\x85\xC0\x75\x33\x33\xC0\x48\x85\xC0\x75\x09\x48\x8B\x05\x00\x00\x00\x00\xEB\x08\x8B\x48\x70\xE8\x00\x00\x00\x00", "xxxxx????xxxxx????xxxxxxxxxxxxxx????xxxxxx????");
				static inline void(*Flush)(char* task) = FetchFunctionFromCall<void(*)(char*)>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x173);
				static inline int(*GetStatus)(const char* name) = FindSignature<int(*)(const char*)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x33\xDB\x48\x8B\xF9\x44\x8B\xC3\x4D\x85\xC0\x75\x09\x4C\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxxxx");
				static inline void(*DestroyPriority)(uint32_t priority) = FindSignature<void(*)(uint32_t)>("\x48\x89\x5C\x24\x10\x56\x48\x83\xEC\x20\x48\x8B\x1D\x00\x00\x00\x00\x8B\xF1\x48\x85\xDB\x74\x69", "xxxxxxxxxxxxx????xxxxxxx");
				static inline char*(*SearchByName)(const char* fileName, int pri) = FindSignature<char* (*)(const char*, int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\x45\x33\xC0\x4D\x85\xC0\x75\x09\x4C\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx????");
				static inline char*(*ReadRequestSub)(const char* name, int pri, size_t size) = FindSignature<char* (*)(const char*, int, size_t)>("\x40\x55\x56\x57\x48\x81\xEC\x30\x01\x00\x00\x48\x8B\x05", "xxxxxxxxxxxxxx");				
				
				static char* Alloc(const char* name, int pri)
				{
					auto _fetchCache = SearchByName(name, pri);

					if (!_fetchCache || *reinterpret_cast<uint32_t*>(_fetchCache + 0x48) >= 10000)
					{
						auto _fetchSize = YS::FILE::GetSize(name);
						_fetchCache = ReadRequestSub(name, pri, _fetchSize);
					}

					strstr(_fetchCache + 0x04, "WM_CURSOR.mdlx");
					*reinterpret_cast<uint32_t*>(_fetchCache + 0x02) += 1;

					return *reinterpret_cast<char**>(_fetchCache + 0x58);
				}
		private:
			static bool _init()
			{
				// RedirectFunction("\x40\x53\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B\xD8\x48\x85\xC0\x74\x09\x81\x78\x48\x10\x27", "xxxxxxx????xxxxxxxxxxxxx", reinterpret_cast<uint64_t>(Alloc), 0x5A);
				return true;
			}

			#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
			static inline bool _doInit = _init();
			#endif
		};
	}
}