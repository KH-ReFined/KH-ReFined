#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COMMAND_ELEM
		{
		public:
			using Get_t = char* (*)(uint64_t id);
			static Get_t Get;

			static char* ReactionID;
			static uint64_t CommandElem;
		};
	}
}