#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YI
	{
		class DLL_EXPORT LAYOUT
		{
		public:
			static inline char* (*GetSequenceUnit)(char* layout, int no) = FindSignature<char*(*)(char*, int)>("\x48\x8B\x41\x30\x48\x63\xD2\x8B\x04\x90\x48\x03\x41\x10\xC3", "xxxxxxxxxxxxxxx");
		};
	}
}