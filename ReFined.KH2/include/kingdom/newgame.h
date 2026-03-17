#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"
#include "message.h"
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

                    auto _fetchInit = SignatureScan<char*>("\x48\x89\x5C\x24\x20\x56\x57\x41\x56\x48\x83\xEC\x30\xE8", "xxxxxxxxxxxxxx");

                    printf("Fetched Title::NewGame::Init @ 0x%p\n", _fetchInit);

                    memset(_fetchInit + 0xE3, 0x04, 0x01);
                    memcpy(_fetchInit + 0x16B, "\xE0\x1E\x00\x00", 0x04);
                    memcpy(_fetchInit + 0x264, "\xE0\x0E\x00\x00", 0x04);

                    printf("Edited Title::NewGame::Init [0x%p] to allocate more space for Re:Fined options.\n", _fetchInit);

                    printf("\nSuccessfully handled Title::NewGame concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
        };
    }
}
