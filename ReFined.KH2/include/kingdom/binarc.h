#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT BINARC
		{
		public:
			using get_info_by_tag_t = char* (*)(char* binarc, uint32_t type, uint32_t tag, int n);
			static get_info_by_tag_t get_info_by_tag;
		};
	}
}