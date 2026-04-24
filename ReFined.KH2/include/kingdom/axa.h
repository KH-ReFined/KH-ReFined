#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace Axa
	{
		class DLL_EXPORT ResourceEntry
		{
		public:
			using ReadDataLocal_t = char*(*)(char* resourceEntry, char* base, const char* folder, const char* file);
			static ReadDataLocal_t ReadDataLocal;
		};
	}
}