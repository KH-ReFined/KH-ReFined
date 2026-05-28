#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <codecvt> 

using namespace std;

struct MODULE_INFO
{
    char* moduleStart;
    char* moduleEnd;

    MODULE_INFO()
    {
        auto hModule = GetModuleHandle(NULL);
        moduleStart = reinterpret_cast<char*>(hModule);

        MODULEINFO _fetchInfo = {};
        GetModuleInformation(GetCurrentProcess(), hModule, &_fetchInfo, sizeof(_fetchInfo));

        moduleEnd = moduleStart + _fetchInfo.SizeOfImage;
    }
};

static const MODULE_INFO moduleInfo;

static bool IsLibraryLinked(const wchar_t* libraryName)
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

                auto _currName = std::wstring(fetchName);

                if (_currName.find(libraryName) != std::wstring::npos)
                    return true;
            }
        }

        return false;
    }
}

template <typename T>
static T FindSignature(const char* pattern, const char* mask)
{
    auto _patternLen = std::strlen(mask);
    auto _maxSearch = moduleInfo.moduleEnd - _patternLen;

    for (char* _currPtr = moduleInfo.moduleStart; _currPtr < _maxSearch; ++_currPtr)
    {
        size_t _procBytes = 0;

        for (; _procBytes < _patternLen; ++_procBytes)
        {
            if (mask[_procBytes] != '?' && pattern[_procBytes] != _currPtr[_procBytes])
                break;
        }

        if (_procBytes == _patternLen)
            return reinterpret_cast<T>(_currPtr);
    }

    return nullptr;
}

template <typename T>
static vector<T> FindAllSignature(const char* pattern, const char* mask)
{
    vector<T> _returnList;

    auto _patternLen = std::strlen(mask);
    auto _maxSearch = moduleInfo.moduleEnd - _patternLen;

    for (char* _currPtr = moduleInfo.moduleStart; _currPtr < _maxSearch; ++_currPtr)
    {
        size_t _procBytes = 0;

        for (; _procBytes < _patternLen; ++_procBytes)
        {
            if (mask[_procBytes] != '?' && pattern[_procBytes] != _currPtr[_procBytes])
                break;
        }

        if (_procBytes == _patternLen)
        {
            _returnList.push_back(reinterpret_cast<T>(_currPtr));
            continue;
        }
    }

    return _returnList;
}

template <typename T>
static T FetchFunctionFromCall(const char* pattern, const char* mask, size_t callOffset = 0)
{
    uint8_t* _fetchSignature = FindSignature<uint8_t*>(pattern, mask);

    if (!_fetchSignature)
        return nullptr;

    _fetchSignature += callOffset;

    if (_fetchSignature[0x00] != 0xE8 && _fetchSignature[0x00] != 0xE9)
        return 0;

    int relOffset;
    memcpy(&relOffset, _fetchSignature + 0x01, sizeof(relOffset));

    return reinterpret_cast<T>(_fetchSignature + 0x05 + relOffset);
}

template <typename T>
static T FetchRelativePointer(const char* pattern, const char* mask, size_t relOffset = 0)
{
    char* _fetchSignature = FindSignature<char*>(pattern, mask);

    if (!_fetchSignature)
        return nullptr;

    int _fetchValue;

    memcpy(&_fetchValue, _fetchSignature + relOffset, sizeof(int));
    return reinterpret_cast<T>(_fetchSignature + _fetchValue + relOffset + 0x04);
}

template <typename T>
static T FetchRelativePointer(char* addr, size_t relOffset = 0)
{
    int _fetchValue;
    std::memcpy(&_fetchValue, addr + relOffset, sizeof(_fetchValue));

    return reinterpret_cast<T>(addr + _fetchValue + relOffset + 0x04);
}

template <typename T>
static T FetchAbsolutePointer(const char* pattern, const char* mask, size_t relOffset = 0)
{
    char* _fetchSignature = FindSignature<char*>(pattern, mask);

    if (!_fetchSignature)
        return nullptr;

    int _fetchValue;

    memcpy(&_fetchValue, _fetchSignature + relOffset, sizeof(int));
    return reinterpret_cast<T>(moduleInfo.moduleStart + _fetchValue);
}

static void RedirectFunction(const char* pattern, const char* mask, uint64_t function, size_t funcLength)
{
    vector<uint8_t> _absoluteInstructionJMP =
    {
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    auto _functionOrig = FindSignature<char*>(pattern, mask);

    memset(_functionOrig, 0x90, funcLength);

    memcpy(_absoluteInstructionJMP.data() + 0x06, &function, 0x08);
    memcpy(_functionOrig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
}

static void RedirectFunction(char* functionPtr, uint64_t function, size_t funcLength)
{
    vector<uint8_t> _absoluteInstructionJMP =
    {
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    memset(functionPtr, 0x90, funcLength);

    memcpy(_absoluteInstructionJMP.data() + 0x06, &function, 0x08);
    memcpy(functionPtr, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
}

static void RedirectRelativeFunction(const char* pattern, const char* mask, uint64_t relOffset, uint64_t function, size_t funcLength)
{
    vector<uint8_t> _absoluteInstructionJMP =
    {
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    auto _functionOrig = FetchRelativePointer<char*>(pattern, mask, relOffset);

    memset(_functionOrig, 0x90, funcLength);

    memcpy(_absoluteInstructionJMP.data() + 0x06, &function, 0x08);
    memcpy(_functionOrig, _absoluteInstructionJMP.data(), _absoluteInstructionJMP.size());
}

static void RedirectLEA(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x07;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x03, &_instCalcAddr, 0x04);
}

static void RedirectMOV(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x06;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x02, &_instCalcAddr, 0x04);
}

static void RedirectMOVZX(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x07;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x03, &_instCalcAddr, 0x04);
}

static void RedirectCMP(char* addr, char* dest)
{
    auto _instEndAddr = addr + 0x07;
    uint32_t _instCalcAddr = dest - _instEndAddr;
    memcpy(addr + 0x02, &_instCalcAddr, 0x04);
}