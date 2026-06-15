#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h" 
#include "converter.h"

extern "C"
{
	namespace ryj
	{
		class DLL_EXPORT PAX
		{
		public:
			static inline char* (*Start)(char* PAX, int EffectID, int Flags, int Type, int Priority, char* Object) = FindSignature<char*(*)(char*, int, int, int, int, char*)>("\x40\x56\x41\x57\x48\x83\xEC\x58\x48\x8B\x01\x48\x8B\xF1\x45\x33\xFF\x48\x63\x48\x08\x85\xC9\x0F\x8E", "xxxxxxxxxxxxxxxxxxxxxxxxx");

			static void Init(char* PAX, char* Data)
			{
				*reinterpret_cast<char**>(PAX + 0x10) = Data + *reinterpret_cast<const uint32_t*>(Data + 0x0C);

				if (!*reinterpret_cast<char**>(PAX + 0x28))
				{
					char* _fetchPaxList = *reinterpret_cast<char**>(ryj::PAX::PaxList);

					*reinterpret_cast<uint32_t*>(PAX + 0x28) = 0x00;
					*reinterpret_cast<uint32_t*>(_fetchPaxList + 0x28) = PC::CONVERTER::POINTER_TO_INTPTR(PAX);

					*reinterpret_cast<char**>(ryj::PAX::PaxList) = PAX;
				}
			}

			static inline char* PaxList = FetchRelativePointer<char*>("\x40\x53\x48\x83\xEC\x20\x4C\x8B\x02\x48\x8B\xD9\x4C\x89\x01\x49\x8D\x40\x10\x48\x89\x41\x08\x48\x83\xC1\x10\x49\x63\x50\x0C\x49\x03\xD0\xE8\x00\x00\x00\x00\x48\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????x", 0x2A) + 0x01;
		};
	}
}