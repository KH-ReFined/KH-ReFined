#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "message.h"
#include "hookintro.h"
#include "sequence.h"
#include "panacea_alloc.h"
#include "sprite.h"
#include "menuutil.h"
#include "select.h"

extern "C"
{

    namespace Title
    {
        class DLL_EXPORT NewGame
        {
        public:
            static char* SetupResult();
            static char* SetupConfig();

            static char** m_SeqTbl;
            static int* m_pri;
            static char** m_Lay;
            static char** m_SelPtr;
            static char** m_MenuPtr;
            static char* s_Plate2Seq;

            struct staticInitializer
            {
                staticInitializer()
                {
                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning Title::NewGame...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _setupResult_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx");
                    uint64_t _constFunction = reinterpret_cast<uint64_t>(SetupResult);

                    printf("Fetched Title::NewGame::SetupResult @ 0x%p\n", _setupResult_orig);

                    fill(_setupResult_orig, _setupResult_orig + 0x41E, 0x90);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
                    memcpy(_setupResult_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked Title::NewGame::SetupResult [0x%p] to Re:Fined function @ 0x%p\n", _setupResult_orig, SetupResult);

                    printf("\nSuccessfully handled Title::NewGame concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}
