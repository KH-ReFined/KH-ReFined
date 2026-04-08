#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <algorithm>

#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "image.h"
#include "sequence.h"
#include "cache_buff.h"

extern "C"
{
	namespace dk {

		class DLL_EXPORT MISSION_GAUGE {
		public:
			using create_t = void(*)(char* self, char* imd, char* sqd, int align, float gaugeMax, int* gaugeNow, int pri);
			static create_t create;

			static void updateGauge(char* missionGauge);

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::MISSION_GAUGE...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constDraw = (uint64_t)updateGauge;
                    auto _draw_orig = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\x0F\x29\x74\x24\x20\x48\x81\xC1\x08\x09\x00", "xxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::INFORMATION_WINDOW::updateGauge @ 0x%p\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0x95);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::MISSION_GAUGE::updateGauge [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, updateGauge);

                    printf("\nSuccessfully handled dk::MISSION_GAUGE concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}