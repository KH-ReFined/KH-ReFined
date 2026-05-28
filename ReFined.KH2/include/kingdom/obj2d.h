#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include "info_base.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Obj2D
		{
		public:
			static inline bool(*isExist)(char* obj) = FetchFunctionFromCall<bool(*)(char*)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0x18);
			static inline void(*commitSuicide)(char* obj, int unk1) = FindSignature<void(*)(char*, int)>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x89\x91\xE0\x01\x00\x00\xE8\x00\x00\x00\x00\x48\x83\xBB\xE0\x01\x00\x00\x00\x74\x00\x48\x89\x7C\x24\x30", "xxxxxxxxxxxxxxxxx????xxxxxxxxx?xxxxx");
			static inline void(*create)(char* obj, int priority, int group) = FetchRelativePointer<void(*)(char*, int, int)>(reinterpret_cast<char*>(dk::INFO_BASE::create), 0x21);

			static inline char** sm_manager_ptr = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x44\x89\x41\x14\x4C\x8D", "xxxxxxxxxxxxxxxx", 0x36);
		};
	}
}