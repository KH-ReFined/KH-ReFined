#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COMMAND_ELEM
		{
		public:
			using Get_t = char* (*)(uint64_t id);
			static Get_t Get;

			// Originally GetForm
			using GetDrive_t = char* (*)(uint64_t form);
			static GetDrive_t GetDrive;

			static char* ReactionID;
			static uint64_t CommandElem;
		};
	}
}