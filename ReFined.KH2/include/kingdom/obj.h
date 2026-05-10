#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "collision_data.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT OBJ
		{
		public:
			using is_exist_t = bool(*)(char* obj);
			using is_enable_collision_t = bool(*)(char* obj, uint32_t group);
			using get_bone_matrix_t = float(*)(char* obj, uint32_t bone, char* matrix);
			using get_part_num_t = uint16_t(*)(char* obj);

			static is_exist_t is_exist;
			static get_part_num_t get_part_num;
			static is_enable_collision_t is_enable_collision;
			static get_bone_matrix_t get_bone_matrix;

            static float get_top_matrix(char* obj, char* matrix);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning YS::OBJ...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constGetTopMatrix = (uint64_t)get_top_matrix;
                    auto _getTopMatrix_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x33\xDB\x48\x8B\xF2\x48\x8B\xF9\x48\x39\x99", "xxxxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched YS::OBJ::get_top_matrix @ 0x%p\n", _getTopMatrix_orig);

                    fill(_getTopMatrix_orig, _getTopMatrix_orig + 0x65, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constGetTopMatrix, 0x08);
                    memcpy(_getTopMatrix_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked YS::OBJ::get_top_matrix [0x%p] to Re:Fined function @ 0x%p\n", _getTopMatrix_orig, get_top_matrix);

                    printf("\nSuccessfully handled YS::OBJ concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}