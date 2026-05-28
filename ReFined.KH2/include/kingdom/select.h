#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT Select
		{
			
		public:
			static inline void(*SetCurrent)(char* select, int pos, bool isSet, int num, int next) = FindSignature<void(*)(char*, int, bool, int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\x66\x89\x11\x0F\xB7\x49\x04", "xxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline int (*SetSelectTop)(char* select, int pos) = FindSignature<int(*)(char*, int)>("\x4C\x8B\xC9\x66\x89\x51\x12\x4C\x8D\x41\x12\x0F\xB7\xCA\x41\x80\x79\x25\x00", "xxxxxxxxxxxxxxxxxxx");
			static inline void (*SetItem)(char* select, int pos, uint32_t s_unit, uint32_t i_unit, char* layout, int message, uint16_t* seq_n, char* seq_a) = FindSignature<void(*)(char*, int, uint32_t, uint32_t, char*, int, uint16_t*, char*)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x40\x48\x8B\x9C\x24\x80\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx");
			static inline char* (*GetItemPtr)(char* select, uint64_t pos) = FindSignature<char*(*)(char*, uint64_t)>("\x48\x8B\x41\x28\x48\x63\xD2\x48\x8B\x04\xD0\xC3", "xxxxxxxxxxxx");
			static inline int (*GetSelectPos)(char* select) = FindSignature<int(*)(char*)>("\x0F\xB7\x51\x16\x0F\xBF\x41\x12\x0F\xBF\x09\x03\xC1\x66\x85\xD2\x74\x08\x0F\xBF\xCA\x99\xF7\xF9\x8B\xC2\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void (*SetExOffset)(char* select, int pos, int x, int y) = FindSignature<void(*)(char*, int, int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\x41\x28", "xxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void (*SetPageMode)(char* select, int mode) = FindSignature<void(*)(char*, int)>("\x44\x8B\x41\x40\x41\x8B\xC0\x25\xFF\xCF\xFF\xFF\x88\x51\x27\x41", "xxxxxxxxxxxxxxxx");
			static inline void (*SetSelectMax)(char* select, int max) = FindSignature<void(*)(char*, int)>("\x85\xD2\x79\x09\x0F\xB7\x41\x04\x66\x89\x41\x16\xC3\x66\x89\x51", "xxxxxxxxxxxxxxxx");
		};
	}
}