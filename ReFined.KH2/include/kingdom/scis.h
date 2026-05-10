#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"
#include "messagedraw.h"
#include "sound.h"
#include "panacea_alloc.h"

extern "C"
{
	namespace dk {

		class DLL_EXPORT Scis 
		{
			public:
				static void setScis(char* scissor, short x, short y, short width, short height);

                struct staticInitializer
                {
                    staticInitializer()
                    {
                        #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                            return;
                        #endif

                        printf("======================================================\n");
                        printf("Handling hooks and redirections concerning dk::Scis...\n\n");

                        vector<uint8_t> _absoluteInstructionJMP =
                        {
                            0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                        };

                        auto _constDraw = (uint64_t)setScis;
                        auto _draw_orig = SignatureScan<char*>("\x66\x89\x54\x24\x08\x66\xFF\xCA\x66\x44\x89\x44\x24\x0C\x66\x41", "xxxxxxxxxxxxxxxx");

                        printf("Fetched dk::Scis::setScis @ 0x%p\n", _draw_orig);

                        memset(_draw_orig, 0x90, 0x21);

                        memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                        memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                        printf("Hooked dk::Scis::setScis [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, setScis);

                        printf("\nSuccessfully handled dk::Scis concerns.\n");
                        printf("======================================================\n\n");
                    }
                };

                static staticInitializer initialize;
		};
	}
}