#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Vsync
		{
		public:
			static inline void(*setLimit)(int limit) = FindSignature<void(*)(int)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8D\x79\x01\x8B\xD9\x8B\xCF\xE8\x00\x00\x00\x00\xF3\x0F\x10\x0D\x00\x00\x00\x00\x33\xC0\x00\x00\x00\x00\x00\x00\x0F\x57\xC0\x89\x05\x00\x00\x00\x00\x0F\x57\xD2\x83\xFF\x08", "xxxxxxxxxxxxxxxxxx????xxxx????xx??????xxxxx????xxxxxx");

			static inline bool* IsFrameLimited = FetchRelativePointer<bool*>(reinterpret_cast<char*>(*dk::Vsync::setLimit), 0x22);
			static inline float* GameSpeed = FetchRelativePointer<float*>(reinterpret_cast<char*>(*dk::Vsync::setLimit), 0x8E);
		};
	}
}