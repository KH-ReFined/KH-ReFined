#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"
#include "cmcustom.h"

extern "C"
{
	class DLL_EXPORT WORLD
	{
	public:
		using GetName_t = char* (*)(int world);
		static GetName_t GetName;
	};
}