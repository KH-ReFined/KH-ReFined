#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "memorymgr.h"
#include <cstdint>

extern "C"
{
	namespace kn
	{
		class DLL_EXPORT FVector
		{
		public:
			static uint32_t* Init(uint32_t* _firstPtr, uint32_t* _secondPtr);

            struct staticInitializer
            {
                staticInitializer()
                {
                    #if defined(BUILD_ARCHIPELAGO) || defined(BUILD_ARCHIPELAGO_LITE)
                        return;
                    #endif

                    printf("======================================================\n");
                    printf("Handling hooks and redirections concerning kn::FVector...\n\n");

                    vector<uint8_t> _absoluteInstructionJMP =
                    {
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    auto _initFunc = (uint64_t)Init;
                    auto _init_orig = SignatureScan<char*>("\x48\x3B\xCA\x74\x16\x8B\x02\x89\x01", "xxxxxxxxx");

                    printf("Fetched kn::FVector::Init @ 0x%p\n", _init_orig);

                    memset(_init_orig, 0x90, 0x1F);

                    memcpy(_absoluteInstructionJMP.data() + 0x06, &_initFunc, 0x08);
                    memcpy(_init_orig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());

                    printf("Hooked kn::FVector::Init [0x%p] to Re:Fined function @ 0x%p\n", _init_orig, Init);

                    printf("\nSuccessfully handled kn::FVector concerns.\n");
                    printf("======================================================\n\n");
                }
            };

            static staticInitializer initialize;
		};
	}
}