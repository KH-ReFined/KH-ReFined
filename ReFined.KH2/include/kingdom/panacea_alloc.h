#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <map>
#include "memorymgr.h"

using namespace std;

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT PANACEA_ALLOC
        {
        private:
            static inline char* (*createAllocator)(const char* _baseAddr, size_t size) = FindSignature<char* (*)(const char*, size_t)>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8D\x59\x0F\x33\xFF\x48\x83\xE3\xF0\x4C\x8D\x0C\x11\x4C\x8D\x83\x80\x00\x00\x00\x49\x8D\x40\x30\x4C\x3B\xC8\x0F\x86\x8E\x00\x00\x00\x48\x39\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");
            static inline char* _allocInstance = createAllocator(moduleInfo.moduleStart + 0x7B0000, 0x10000);

            static inline uint64_t _functionSpace = *reinterpret_cast<const uint64_t*>(_allocInstance);

            static inline char* (*alloc)(char* instance, size_t size) = reinterpret_cast<char*(*)(char*, size_t)>(*reinterpret_cast<char**>(_functionSpace + 0x08));
            static inline void(*free)(char* instance, char* address) = reinterpret_cast<void(*)(char*, char*)>(*reinterpret_cast<char**>(_functionSpace + 0x10));

        public:
            static inline map<string, char*> MEMORY_BLOCK = map<string, char*>();

            static char* Get(string Key)
            {
                auto tagCheck = MEMORY_BLOCK.find(Key) != MEMORY_BLOCK.end();

                if (!tagCheck)
                    return 0x00;

                uint64_t tagValue = *reinterpret_cast<const uint64_t*>(MEMORY_BLOCK.at(Key));

                bool tagValid = tagValue != 0xCAFEEFACCAFEEFAC &&
                    tagValue != 0xEFACCAFEEFACCAFE;

                return tagValid ? MEMORY_BLOCK.at(Key) : nullptr;
            }

            static void Free(string Key)
            {
                auto _checkMemory = MEMORY_BLOCK.find(Key) != MEMORY_BLOCK.end();

                if (_checkMemory)
                {
                    free(_allocInstance, MEMORY_BLOCK[Key]);
                    MEMORY_BLOCK.erase(Key);
                }
            }

            static void Allocate(string Key, size_t Size)
            {
                DWORD lpflOldProtect = 0;

                MEMORY_BLOCK[Key] = alloc(_allocInstance, Size);
                VirtualProtect(MEMORY_BLOCK[Key], Size, PAGE_EXECUTE_READWRITE, &lpflOldProtect);

                memset(MEMORY_BLOCK[Key], 0x00, Size);
            }
        };
    }
}