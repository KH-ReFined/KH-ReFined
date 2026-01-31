#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <map>
#include <string>
#include "SigScan.h"

using namespace std;

extern "C"
{
    namespace YS
    {
        class DLL_EXPORT PANACEA_ALLOC
        {
        public:
            static map<string, char*> MEMORY_BLOCK;

            using alloc_t = char* (*)(char* instance, size_t size);
            static alloc_t alloc;

            using free_t = void(*)(char* instance, char* address);
            static free_t free;

            static char* _allocInstance;
            static uint64_t _functionSpace;

            static char* Get(string Key);
            static void Free(string Key);
            static void Allocate(string  Key, size_t Size);
        };
    }
}