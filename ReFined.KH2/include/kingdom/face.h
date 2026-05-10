#pragma once

#define DLL_EXPORT __declspec(dllexport)
#include <cstdint>

#include "area.h"
#include "obj2d.h"
#include "binarc.h"
#include "sprite.h"
#include "image.h"
#include "menu.h"
#include "image_cache.h"
#include "converter.h"
#include "memorymgr.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT FACE
        {
        public:
            using getFaceSed_t = char (*)(char* face, char** sqd, char* object);
            static getFaceSed_t getFaceSed;

            static void create(char* face, int priority, int type, char* object, int status, int group, int offset16x9);
            static void reload(char* face, char* object, int status);
            static void draw(char* face);
            static void update(char* face);

            static void emptyDraw(char* sprite);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning dk::FACE...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };
                    
                    auto _createFunc = (uint64_t)create;
                    auto _create_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x8B\xF2\x44\x89\x81\x00\x03\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::FACE::create @ 0x%p.\n", _create_orig);

                    memset(_create_orig, 0x90, 0xBF);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_createFunc, 0x08);
                    memcpy(_create_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::FACE::create [0x%p] to Re:Fined function [0x%p].\n", _create_orig, create);

                    auto _updateFunc = (uint64_t)update;
                    auto _update_orig = SignatureScan<char*>("\x80\xB9\x04\x03\x00\x00\x00\x0F\x85", "xxxxxxxxx");

                    printf("Fetched dk::FACE::update @ 0x%p.\n", _update_orig);

                    memset(_update_orig, 0x90, 0x0E);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_updateFunc, 0x08);
                    memcpy(_update_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::FACE::update [0x%p] to Re:Fined function [0x%p].\n", _update_orig, update);

                    auto _drawFunc = (uint64_t)draw;
                    auto _draw_orig = SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x80\xB9\x04\x03\x00\x00\x00\x48\x8B\xD9\x74\x19\x48\x81\xC1", "xxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::FACE::draw @ 0x%p.\n", _draw_orig);

                    memset(_draw_orig, 0x90, 0x35);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_drawFunc, 0x08);
                    memcpy(_draw_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::FACE::draw [0x%p] to Re:Fined function [0x%p].\n", _draw_orig, draw);

                    auto _reloadFunc = (uint64_t)reload;
                    auto _reload_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x41\x8B\xD8\x33\xFF\x4C\x8B\xC2\x48", "xxxxxxxxxxxxxxxxxxxxxxxx");

                    printf("Fetched dk::FACE::reload @ 0x%p.\n", _reload_orig);

                    memset(_reload_orig, 0x90, 0x35);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_reloadFunc, 0x08);
                    memcpy(_reload_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked dk::FACE::reload [0x%p] to Re:Fined function [0x%p].\n", _reload_orig, reload);

                    printf("\nSuccessfully handled dk::FACE concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}