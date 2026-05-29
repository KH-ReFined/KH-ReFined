#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "image.h"
#include "sequence.h"
#include "mission_gauge.h"
#include "mission_count.h"

extern "C"
{
	namespace dk 
	{
		class DLL_EXPORT INFORMATION 
		{
		public:
			static inline void(*openInformationWindow)(const char* message) = FindSignature<void(*)(const char*)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x48\x8B\xD3", "xxxxxxxxxxxx????x????xxx????xxx");
			static inline void(*openGetMunny)(int munny) = FetchFunctionFromCall<void(*)(int)>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x41\x0F\xB6\x58", "xxxxxxxxxxxxxxxxxxxxxxxx", 0xB1);
			
			static inline void(*forceLeaveCounter)(char* counter);
		};
	}
}