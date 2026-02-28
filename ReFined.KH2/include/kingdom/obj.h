#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "SigScan.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJ
		{
		public:
			using is_exist_t = bool(*)(char* obj);
			using get_part_num_t = uint16_t(*)(char* obj);

			static is_exist_t is_exist;
			static get_part_num_t get_part_num;
		};
	}
}