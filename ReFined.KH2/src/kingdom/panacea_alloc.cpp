#include "panacea_alloc.h"

using createAllocator_t = char* (*)(const char* _baseAddr, size_t size);
createAllocator_t createAllocator = SignatureScan<createAllocator_t>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8D\x59\x0F\x33\xFF\x48\x83\xE3\xF0\x4C\x8D\x0C\x11\x4C\x8D\x83\x80\x00\x00\x00\x49\x8D\x40\x30\x4C\x3B\xC8\x0F\x86\x8E\x00\x00\x00\x48\x39\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????");

char* YS::PANACEA_ALLOC::_allocInstance = createAllocator(moduleInfo.startAddr + 0x7B0000, 0x10000);
uint64_t YS::PANACEA_ALLOC::_functionSpace = *reinterpret_cast<const uint64_t*>(_allocInstance);

YS::PANACEA_ALLOC::free_t  YS::PANACEA_ALLOC::free = *reinterpret_cast<const free_t*>(_functionSpace + 0x10);
YS::PANACEA_ALLOC::alloc_t YS::PANACEA_ALLOC::alloc = *reinterpret_cast<const alloc_t*>(_functionSpace + 0x08);

map<string, char*> YS::PANACEA_ALLOC::MEMORY_BLOCK = map<string, char*>();

void YS::PANACEA_ALLOC::Allocate(string Key, size_t Size)
{
    DWORD lpflOldProtect = 0;

    MEMORY_BLOCK[Key] = alloc(_allocInstance, Size);
    VirtualProtect(MEMORY_BLOCK[Key], Size, PAGE_EXECUTE_READWRITE, &lpflOldProtect);

    fill(MEMORY_BLOCK[Key], MEMORY_BLOCK[Key] + Size, 0x00);
}

void YS::PANACEA_ALLOC::Free(string Key)
{
    auto _checkMemory = MEMORY_BLOCK.find(Key) != MEMORY_BLOCK.end();

    if (_checkMemory)
    {
        free(_allocInstance, MEMORY_BLOCK[Key]);
        MEMORY_BLOCK.erase(Key);
    }
}

char* YS::PANACEA_ALLOC::Get(string Key)
{
    auto tagCheck = MEMORY_BLOCK.find(Key) != MEMORY_BLOCK.end();

    if (!tagCheck)
        return 0x00;

    uint64_t tagValue = *reinterpret_cast<const uint64_t*>(MEMORY_BLOCK.at(Key));

    bool tagValid = tagValue != 0xCAFEEFACCAFEEFAC &&
        tagValue != 0xEFACCAFEEFACCAFE;

    return tagValid ? MEMORY_BLOCK.at(Key) : nullptr;
}