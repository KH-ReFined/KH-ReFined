#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT INFO_BASE
		{
		public:
			static inline void(*update)(char* base) = FindSignature<void(*)(char*)>("\x48\x8B\xC4\x53\x48\x81\xEC\x80\x00\x00\x00\xF3\x0F\x10\x41\x30", "xxxxxxxxxxxxxxxx");
			static inline void(*create)(char* base, int priority, int height, int draw_priority, int group, int align) = FetchFunctionFromCall<void(*)(char*, int, int, int, int, int)>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0x124);

			static inline char* sm_height = FetchRelativePointer<char*>(reinterpret_cast<char*>(dk::INFO_BASE::create), 0x2D);
		};
	}
}