#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT REGION
		{
		public:
			static inline uint32_t(*Get)() = FetchFunctionFromCall<uint32_t(*)()>("\x40\x53\x48\x83\xEC\x30\x48\x83\x79\x40\x00\x48\x8B\xD9\x75\x28", "xxxxxxxxxxxxxxxx", 0x3F);

			static inline char** Region = FetchRelativePointer<char**>("\x83\xF9\x07\x0F\x87\xDC\x00\x00\x00\x48\x63\xC1", "xxxxxxxxxxxx", 0x37);
			static inline char** DefaultRegion = FetchRelativePointer<char**>("\x83\xF9\x07\x0F\x87\xDC\x00\x00\x00\x48\x63\xC1", "xxxxxxxxxxxx", 0x29);
		};
	}
}