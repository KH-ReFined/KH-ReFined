#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MEMBER
		{
		public:
			static inline uint32_t(*PartToEntryID)(uint32_t part) = FetchFunctionFromCall<uint32_t(*)(uint32_t)>("\x44\x89\x4C\x24\x20\x48\x89\x4C\x24\x08\x53\x56\x57\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x38", "xxxxxxxxxxxxxxxxxxxxxxx", 0x108);
			static inline char* (*Get)(int world) = FindSignature<char* (*)(int)>("\x85\xC9\x79\x07\x0F\xB6\x0D\x00\x00\x00\x00\x48\x63\xC1", "xxxxxxx????xxx");

			static int PartToMemberPart(int part)
			{
				switch (part)
				{
				case 3:
					return 2;
					break;
				case 2:
					return 1;
					break;
				case 1:
				case 14:
					return 0;
					break;
				default:
					return 3;
					break;
				}
			}
		};
	}
}