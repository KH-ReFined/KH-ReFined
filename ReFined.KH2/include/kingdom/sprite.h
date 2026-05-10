#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>
#include "memorymgr.h"
#include "obj2d.h"
#include "sequence.h"
#include "panacea_alloc.h"
#include "menu.h"

extern "C"
{
	namespace dk
	{
		class DLL_EXPORT Sprite
		{
		public:
			using setNum_t = void(*)(char* Sprite, int num);
			using create_t = void(*)(char* Sprite, int priority, char* seqd, char* image, int num, int group, int offset16x9);
			using setPos_t = void(*)(char* Sprite, int x, int y);
			using initSqd_t = void(*)(char* Sprite, char* sqd, char* image, int num);
            using update_t = void(*)(char* Sprite);

			static setNum_t setNum;
			static setPos_t setPos;
			static update_t update;
            static initSqd_t initSqd;

			static void _Sprite(char* self);
			static void initWork(char* Sprite);
			static void draw(char* Sprite);
			static void create(char* Sprite, int priority, char* seqd, char* image, int num, int group, int offset16x9);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::Sprite...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _constCreate = (uint64_t)create;
                    auto _create_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x83\xEC\x20\x4D\x8B\xF0\x49\x8B\xE9", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::Sprite::create @ 0x%p\n", _create_orig);

                    memset(_create_orig, 0x90, 0x106);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constCreate, 0x08);
                    memcpy(_create_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::Sprite::create [0x%p] to Re:Fined function @ 0x%p\n", _create_orig, create);

                    auto _constDraw = (uint64_t)draw;
                    auto _draw_orig = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x30\x8B\x41\x10\x48\x8B\xD9\xC1\xE8\x0B\xA8\x01", "xxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::Sprite::draw @ 0x%p\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0x1FD);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constDraw, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::Sprite::draw [0x%p] to Re:Fined function @ 0x%p\n", _draw_orig, draw);

                    printf("\nSuccessfully handled dk::Sprite concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}