#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"

#include <cstdint>
#include <Windows.h>

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT COMMAND_MENU
		{
		public:

			using make_by_table_t = char* (*)(char* commandMenu, char* table, size_t size, char* target);
			static make_by_table_t make_by_table;

			using add_t = char* (*)(char* commandMenu, int command, char* target, int amount);
			static add_t add;
		};
	}
}