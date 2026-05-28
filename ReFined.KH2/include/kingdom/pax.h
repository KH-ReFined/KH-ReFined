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
			static inline void(*Start)(char* PAX, int EffectID, int Flags, int Type, int Priority) = FindSignature<void(*)(char*, int, int, int, int)>("\x48\x83\xEC\x38\x4C\x8D\x91\x80\x00\x00\x00\x49\x83\x3A\x00\x75\x07\x33\xC0\x48\x83\xC4\x38\xC3\x48\x8B\x44\x24\x60", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			static inline char* (*StartBind)(char* PAX, int EffectID, int Flags, int Type, int Priority, char* Object) = FindSignature<char*(*)(char*, int, int, int, int, char*)>("\x40\x56\x41\x57\x48\x83\xEC\x58\x48\x8B\x01\x48\x8B\xF1\x45\x33\xFF\x48\x63\x48\x08\x85\xC9\x0F\x8E", "xxxxxxxxxxxxxxxxxxxxxxxxx");

			static void Init(char* PAX, char* Data)
			{
				uint64_t _paxAddress = reinterpret_cast<uint64_t>(PAX);
				uint64_t _dataAddress = reinterpret_cast<uint64_t>(Data);

				uint64_t _readInit = *reinterpret_cast<const uint64_t*>(PAX + 0x28);
				uint64_t _fetchPaxStart = _dataAddress + *reinterpret_cast<const uint32_t*>(Data + 0x0C);

				memcpy(PAX + 0x10, &_fetchPaxStart, 0x08);

				if (_readInit == 0x00)
				{
					uint32_t _convertPAX = PC::CONVERTER::POINTER_TO_INTPTR(ryj::PAX::PaxList);
					uint32_t _literalZero = 0x00;

					memcpy(ryj::PAX::PaxList + 0x28, &_convertPAX, 0x04);
					memcpy(PAX + 0x28, &_literalZero, 0x04);

					memcpy(ryj::PAX::PaxList, &_paxAddress, 0x08);
				}
			}

			static inline char* PaxList = FetchRelativePointer<char*>("\x40\x53\x48\x83\xEC\x20\x4C\x8B\x02\x48\x8B\xD9\x4C\x89\x01\x49\x8D\x40\x10\x48\x89\x41\x08\x48\x83\xC1\x10\x49\x63\x50\x0C\x49\x03\xD0\xE8\x00\x00\x00\x00\x48\x83\x3D\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx????x", 0x2A) + 0x01;
		};
	}
}