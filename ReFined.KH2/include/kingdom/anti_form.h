#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <Windows.h>

#include "area.h"
#include "party.h"
#include "form_level.h"
#include "command_elem.h"
#include "item.h"
#include "memorymgr.h"
#include "sacrifice.h"
#include "progress.h"
#include "pref.h"
#include "mission.h"

extern "C"
{
	namespace YS
	{
		class DLL_EXPORT ANTI_FORM
		{
		public:
			static int CheckForm(char* player, int form);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning YS::ANTI_FORM...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _checkForm_orig = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xF2\x48\x8B\xE9\xE8\x00\x00\x00\x00\x85\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxxx????xx");
                    uint64_t _constFunction = reinterpret_cast<uint64_t>(CheckForm);

                    printf("Fetched YS::ANTI_FORM::CheckForm @ 0x%p\n", _checkForm_orig);

                    memset(_checkForm_orig, 0x90, 0x228);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_constFunction, 0x08);
                    memcpy(_checkForm_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked YS::ANTI_FORM::CheckForm [0x%p] to Re:Fined function @ 0x%p\n", _checkForm_orig, CheckForm);

                    printf("\nSuccessfully handled YS::ANTI_FORM concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}