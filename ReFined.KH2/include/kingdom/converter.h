#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace PC
	{
		class DLL_EXPORT CONVERTER
		{
		public:
			static inline char*(*INTPTR_TO_POINTER)(uint32_t) = FindSignature<char*(*)(uint32_t)>("\x40\x53\x48\x83\xEC\x20\x8B\xD9\xE8\x00\x00\x00\x00\x0F\xBA\xF3\x1F\x48\x8D\x15\x00\x00\x00\x00", "xxxxxxxxx????xxxxxxx????");
			static inline uint32_t(*POINTER_TO_INTPTR)(char*) = FindSignature<uint32_t(*)(char*)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xC9\x75\x08\x33\xC0\x48\x83\xC4\x20\x5B\xC3\xE8\x65", "xxxxxxxxxxxxxxxxxxxxxxxx");
		};
	}
}