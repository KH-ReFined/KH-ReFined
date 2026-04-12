#pragma once

#define DLL_EXPORT __declspec(dllexport)
#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <Windows.h>
#include "memorymgr.h"
#include "sound.h"
#include "info_base.h"
#include "obj2d.h"
#include "sprite.h"
#include "sequence.h"
#include "information.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT COUNTER
		{
		public:
			using forceLeave_t = void(*)(char* counter);
			static forceLeave_t forceLeave;

			static void update(char* counter); 

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::COUNTER...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _updateFunc = (uint64_t)update;
                    auto _update_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x55\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x50\x48\x8B\x05", "xxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::COUNTER::update @ 0x%p\n", _update_orig);

                    memset(_update_orig, 0x90, 0x38E);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_updateFunc, 0x08);
                    memcpy(_update_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::COUNTER::update [0x%p] to Re:Fined function @ 0x%p\n", _update_orig, update);

                    printf("\nSuccessfully handled dk::COUNTER concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}