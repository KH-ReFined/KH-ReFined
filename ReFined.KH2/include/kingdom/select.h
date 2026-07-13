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
			static inline void (*_Select)(char* select, short max, bool loop) = FindSignature<void(*)(char*, short, bool)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x33\xED\x66\x89\x51\x10\x44\x88\x41\x24\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline char* (*Alloc)(int size) = FetchFunctionFromCall<char* (*)(int)>("\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\xBF\x05\x00\x00\x00\xBA\x03\x00\x00\x00\x8B\xCF", "xxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x23);
			static inline int(*select)(char* select) = FindSignature<int(*)(char*)>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\x98\x00\x00\x00\x48", "xxxxxxxxxxxxxxxx");
			static inline bool(*isLoop)(char* select, int no) = FindSignature<bool(*)(char*, int)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xF9\x48\x63\xDA\x48\x8B\x49\x28\x48\x8D\x34\xDD\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*SetCurrent)(char* select, int pos, bool isSet, int num, int next) = FindSignature<void(*)(char*, int, bool, int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\x66\x89\x11\x0F\xB7\x49\x04", "xxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline int (*SetSelectTop)(char* select, int pos) = FindSignature<int(*)(char*, int)>("\x4C\x8B\xC9\x66\x89\x51\x12\x4C\x8D\x41\x12\x0F\xB7\xCA\x41\x80\x79\x25\x00", "xxxxxxxxxxxxxxxxxxx");
			static inline void (*SetItem)(char* select, int pos, uint32_t s_unit, uint32_t i_unit, char* layout, int message, uint16_t* seq_n, uint16_t* seq_a) = FindSignature<void(*)(char*, int, uint32_t, uint32_t, char*, int, uint16_t*, uint16_t*)>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x40\x48\x8B\x9C\x24\x80\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx");
			static inline char* (*GetItemPtr)(char* select, uint64_t pos) = FindSignature<char*(*)(char*, uint64_t)>("\x48\x8B\x41\x28\x48\x63\xD2\x48\x8B\x04\xD0\xC3", "xxxxxxxxxxxx");
			static inline int (*GetSelectPos)(char* select) = FindSignature<int(*)(char*)>("\x0F\xB7\x51\x16\x0F\xBF\x41\x12\x0F\xBF\x09\x03\xC1\x66\x85\xD2\x74\x08\x0F\xBF\xCA\x99\xF7\xF9\x8B\xC2\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void (*SetExOffset)(char* select, int pos, int x, int y) = FindSignature<void(*)(char*, int, int, int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\x41\x28", "xxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void (*SetPageMode)(char* select, int mode) = FindSignature<void(*)(char*, int)>("\x44\x8B\x41\x40\x41\x8B\xC0\x25\xFF\xCF\xFF\xFF\x88\x51\x27\x41", "xxxxxxxxxxxxxxxx");
			static inline void (*SetSelectMax)(char* select, int max) = FindSignature<void(*)(char*, int)>("\x85\xD2\x79\x09\x0F\xB7\x41\x04\x66\x89\x41\x16\xC3\x66\x89\x51", "xxxxxxxxxxxxxxxx");
			static inline void (*Loop)(char* select) = FindSignature<void(*)(char*)>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\xD9", "xxxxxxxxxxxxxxxxxx");
			static inline bool (*isExist)(char* select) = FetchFunctionFromCall<bool (*)(char*)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xBA\x01\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xB9\xF6\x2E\x00\x00", "xxxxxxxxxxxxxxxx????x????xxxxxxxxx????x????xxxxx", 0x56);
		
			static void SetMoveKey(char* select, uint16_t up, uint16_t down, uint16_t left, uint16_t right)
			{
				*reinterpret_cast<uint16_t*>(select + 0x0C) = right;
				*reinterpret_cast<uint16_t*>(select + 0x0A) = left;
				*reinterpret_cast<uint16_t*>(select + 0x06) = up;
				*reinterpret_cast<uint16_t*>(select + 0x08) = down;
			}

			static bool isCursorMove(char* select)
			{
				short _fetchPrevious = -1;
				auto _fetchCurrent = *reinterpret_cast<short*>(select + 0x02);

				if (_fetchCurrent < 0x00)
					return false;

				if (*reinterpret_cast<short*>(select + 0x16) < 0x00)
					_fetchPrevious = (*reinterpret_cast<short*>(select) + *reinterpret_cast<short*>(select + 0x12)) % *reinterpret_cast<short*>(select + 0x16);

				else
					_fetchPrevious = *reinterpret_cast<short*>(select) + *reinterpret_cast<short*>(select + 0x12);

				return _fetchPrevious != _fetchCurrent;
			}
		};
	}
}