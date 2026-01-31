#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "SigScan.h"
extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT CmConfig
		{
		public:
			using UpdateList_t = void (*)();
			static UpdateList_t UpdateList;

			using UpdateActive_t = void (*)();
			static UpdateActive_t UpdateActive;
		};
	}
}