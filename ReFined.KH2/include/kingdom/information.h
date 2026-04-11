#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include "obj2d.h"
#include "sprite.h"
#include "image.h"
#include "sequence.h"
#include "mission_gauge.h"

extern "C"
{
	namespace dk {

		class DLL_EXPORT INFORMATION {
		public:
			using openInformationWindow_t = void(*)(const char* message);
			static openInformationWindow_t openInformationWindow;

			using openGetMunny_t = void(*)(int munny);
			static openGetMunny_t openGetMunny;
		};

		class DLL_EXPORT NEXT_EXP
		{
			public:
				static void draw(char* nextExp);


            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::NEXT_EXP...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constDraw = (uint64_t)draw;
                    auto _draw_orig = MultiSignatureScan("\x40\x53\x55\x57\x48\x83\xEC\x30\x48\x8B\xE9\xE8\x00\x00\x00\x00", "xxxxxxxxxxxx????");

                    printf("Fetched dk::NEXT_EXP::draw @ 0x%p and dk::GET_MUNNY::draw @ 0x%p\n", _draw_orig[0], _draw_orig[1]);

                    memset(_draw_orig[0], 0x90, 0xBF);
                    memset(_draw_orig[1], 0x90, 0xBF);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig[0], _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
                    memcpy(_draw_orig[1], _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::NEXT_EXP::draw [0x%p] and dk::GET_MUNNY::draw [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig[0], _draw_orig[1], draw);

                    printf("\nSuccessfully handled dk::NEXT_EXP concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}