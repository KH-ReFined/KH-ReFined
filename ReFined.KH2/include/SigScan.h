#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <locale>
#include <codecvt> 

using namespace std;

struct ModuleInfo
{
    const char* startAddr;
    const char* endAddr;

    ModuleInfo()
    {
        HMODULE hModule = GetModuleHandle(NULL);
        startAddr = reinterpret_cast<const char*>(hModule);

        MODULEINFO info = {};
        GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(info));
        endAddr = startAddr + info.SizeOfImage;
    }
};

static const ModuleInfo moduleInfo;

inline bool FindModule(const char* moduleName)
{
    HMODULE modules[100];
    void* hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, GetCurrentProcessId());

    if (hProcess)
    {
        DWORD bytesNeeded;
        BOOL rc = EnumProcessModules(hProcess, modules, sizeof(modules), &bytesNeeded);

        if (rc)
        {
            int count = (int)(bytesNeeded / sizeof(HMODULE));

            for (int i = 0; i < count; i++)
            {
                wchar_t fetchName[260];
                GetModuleFileName(modules[i], fetchName, 260);

                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> _converter;

                auto _reqName = _converter.from_bytes(moduleName);
                auto _currName = std::wstring(fetchName);

                if (_currName.find(_reqName) != std::wstring::npos)
                    return true;
            }
        }

        return false;
    }
}

template <typename T>
T SignatureScan(const char* pattern, const char* mask)
{
    size_t patLen = std::strlen(mask);

    for (const char* addr = moduleInfo.startAddr; addr < moduleInfo.endAddr - patLen; ++addr)
    {
        size_t i = 0;
        for (; i < patLen; ++i)
        {
            if (mask[i] != '?' && pattern[i] != addr[i])
                break;
        }

        if (i == patLen)
            return reinterpret_cast<T>(const_cast<char*>(addr));
    }

    return nullptr;
}

inline vector<char*> MultiSignatureScan(const char* pattern, const char* mask)
{
    vector<char*> _returnList;
    size_t patLen = std::strlen(mask);

    for (const char* addr = moduleInfo.startAddr; addr < moduleInfo.endAddr - patLen; ++addr)
    {
        size_t i = 0;
        for (; i < patLen; ++i)
        {
            if (mask[i] != '?' && pattern[i] != addr[i])
                break;
        }

        if (i == patLen)
        {
            _returnList.push_back(const_cast<char*>(addr));
            continue;
        }
    }

    return _returnList;
}

inline vector<char*> RangedMultiScan(const char* pattern, const char* mask, char* startAddr, size_t length)
{
    vector<char*> _returnList;
    size_t patLen = std::strlen(mask);

    for (const char* addr = startAddr; addr < (startAddr + length) - patLen; ++addr)
    {
        size_t i = 0;
        for (; i < patLen; ++i)
        {
            if (mask[i] != '?' && pattern[i] != addr[i])
                break;
        }

        if (i == patLen)
        {
            _returnList.push_back(const_cast<char*>(addr));
            continue;
        }
    }

    return _returnList;
}

inline uintptr_t ResolveCallRelative(const void* callInstr)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(callInstr);

    if (bytes[0] != 0xE8 && bytes[0] != 0xE9)
        return 0;

    int32_t relOffset;
    std::memcpy(&relOffset, bytes + 1, sizeof(relOffset));

    return reinterpret_cast<uintptr_t>(bytes + 5 + relOffset);
}

template <typename T>
T ResolveFunctionFromCall(const char* pattern, const char* mask, size_t callOffset = 0)
{
    uint8_t* match = SignatureScan<uint8_t*>(pattern, mask);
    if (!match)
        return nullptr;

    uintptr_t target = ResolveCallRelative(match + callOffset);
    return reinterpret_cast<T>(target);
}

template <typename T>
T ResolveRelativeAddress(const char* pattern, const char* mask, size_t callOffset = 0)
{
    size_t patLen = std::strlen(mask);
    size_t currOffset = 0;

    for (const char* addr = moduleInfo.startAddr; addr < moduleInfo.endAddr - patLen; ++addr)
    {
        size_t i = 0;
        
        for (; i < patLen; ++i)
        {
            if (mask[i] != '?' && pattern[i] != addr[i])
                break;
        }

        if (i == patLen)
        {
            uint32_t _fetchValue;
            std::memcpy(&_fetchValue, addr + callOffset, sizeof(int));
            return reinterpret_cast<T>(const_cast<char*>(_fetchValue + addr + callOffset + 4));
        }

        currOffset++;
    }

    return 0x00;
}

template <typename T>
T ResolveRelativeAddress(const char* addr, size_t callOffset = 0)
{
    int32_t relOffset;
    std::memcpy(&relOffset, addr + callOffset, sizeof(relOffset));

    return reinterpret_cast<T>(const_cast<char*>(addr) + relOffset + callOffset + 0x04);
}


inline char* CalculatePointer(uint64_t Input, initializer_list<int32_t>Offsets)
{
    uint64_t _mainFetch = *reinterpret_cast<uint64_t*>(Input);

    if (_mainFetch == 0x00)
        return 0x00;

    for (int i = 0; i < Offsets.size(); i++)
    {
        uint32_t _currOffset = Offsets.begin()[i];

        if (i != Offsets.size() - 1)
        {
            char* _ptrSub = reinterpret_cast<char*>(_mainFetch) + _currOffset;
            _mainFetch = *reinterpret_cast<uint64_t*>(_ptrSub);
        }

        else
            return reinterpret_cast<char*>(_mainFetch) + _currOffset;
    }
}

inline void RedirectLEA(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x07;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x03, &_instCalcAddr, 0x04);
}

inline void RedirectMOV(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x06;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x02, &_instCalcAddr, 0x04);
}

inline void RedirectMOVZX(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x07;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x03, &_instCalcAddr, 0x04);
}


inline void RedirectCMP(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x07;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x02, &_instCalcAddr, 0x04);
}