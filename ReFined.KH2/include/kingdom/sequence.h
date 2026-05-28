#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT SEQUENCE
		{
		public:
			static inline void(*_SEQUENCE)(char* SEQUENCE) = FetchFunctionFromCall<void(*)(char*)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x167);
			static inline void(*Dispose)(char* SEQUENCE) = FetchFunctionFromCall<void(*)(char*)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xEA\x48\x8D\xB9\x90\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x3A);
			static inline void(*Init)(char* SEQUENCE, char* seqd, char* image) = FetchFunctionFromCall<void(*)(char*, char*, char*)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xD9);
			static inline void(*SetNumber)(char* SEQUENCE, int no) = FetchFunctionFromCall<void(*)(char*, int)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xE6);
			static inline void(*Control)(char* SEQUENCE) = FindSignature<void(*)(char*)>("\x40\x53\x48\x83\xEC\x20\xF3\x0F\x10\x0D\x00\x00\x00\x00\x48\x8B\xD9\xF3\x0F\x59\x49\x68\x0F\x28\xC1\xF3\x0F\x58\x81\x44\x01\x00\x00\xF3\x0F\x11\x81\x44\x01\x00\x00\x8B\x89\x40\x01\x00\x00\x83\xE9\x01\x0F\x84\xA5\x00\x00\x00\x83\xF9\x01\x0F\x85\xD1", "xxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline void(*Draw)(char* SEQUENCE) = FindSignature<void(*)(char*)>("\x4C\x8B\xDC\x53\x48\x81\xEC\xD0\x00\x00\x00\x83\xB9\x40\x01\x00\x00\x00\x48\x8B\xD9", "xxxxxxxxxxxxxxxxxxxxx");
			static inline int(*GetParamCr)(char* SEQUENCE) = FetchFunctionFromCall<int(*)(char*)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0x104);
			static inline void(*SetScaleX)(char* SEQUENCE, float scaleX) = FindSignature<void(*)(char*, float)>("\xF3\x0F\x10\x81\x78\x01\x00\x00\xF3\x0F\x10\x15\x00\x00\x00\x00\x0F\x2E\xC2\xF3\x0F\x11\x89\x7C\x01\x00\x00", "xxxxxxxxxxxx????xxxxxxxxxxx");
			static inline uint32_t(*GetActiveRGBA)(char* SEQUENCE) = FetchFunctionFromCall<uint32_t(*)(char*)>("\x40\x56\x48\x81\xEC\xF0\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\xD0\x00\x00\x00\x48\x8B\xF1\x48\x8B\x89", "xxxxxxxxxxxx????xxxxxxxxxxxxxxxxx", 0xD9);
			static inline void(*SetColorRate)(char* SEQUENCE, float r, float g, float b, float a) = FindSignature<void(*)(char*, float, float, float, float)>("\xF3\x0F\x10\x05\x00\x00\x49\x00\x48\x8B\xD1\x0F\x2E\xC8\xF3\x0F", "xxxx??xxxxxxxxxx");

			static inline void(*SetNumberForce)(char* SEQUENCE, int no) = reinterpret_cast<void(*)(char*, int)>(reinterpret_cast<char*>(YI::SEQUENCE::SetNumber) + 0x30);
			static inline int(*GetParamEx)(char* SEQUENCE) = reinterpret_cast<int(*)(char*)>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamCr) + 0x20);
			static inline int(*GetParamSize)(char* SEQUENCE) = reinterpret_cast<int(*)(char*)>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) + 0x20);
			static inline int(*GetParamX)(char* SEQUENCE) = reinterpret_cast<int(*)(char*)>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) + 0x40);
			static inline int(*GetParamY)(char* SEQUENCE) = reinterpret_cast<int(*)(char*)>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) + 0x60);
			static inline int(*GetActiveX)(char* SEQUENCE) = reinterpret_cast<int(*)(char*)>(reinterpret_cast<char*>(YI::SEQUENCE::GetParamEx) - 0x250);
			static inline int(*GetActiveY)(char* SEQUENCE) = reinterpret_cast<int(*)(char*)>(reinterpret_cast<char*>(YI::SEQUENCE::GetActiveX) + 0x80);

		};
	}
}