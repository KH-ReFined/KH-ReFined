// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include <vector>
#include <Psapi.h>

using namespace std;

vector<uint8_t> _instShortcut
{
	0xEB, 0x19,
	0x3C, 0x0B, 0x75, 0x02, 0xB0,
	0x88, 0x47, 0x01, 0xEB, 0xDC,
	0xCE,
	0xEB, 0xAA,
	0xEB, 0x4E, 0x90, 0x90,
	0x81, 0xCB, 0x00, 0x00, 0x24, 0x00,
	0x80, 0xF9, 0x15, 0x74, 0xF2,
	0xEB, 0x1B, 0x90, 0x90, 0x90, 0x90, 0x90,
	0x31, 0xC0, 0x48, 0x83, 0xC4, 0x28, 0xC3,
	0xEB, 0x45, 0x90, 0x90,
	0x81, 0xC3, 0x00, 0x00, 0x20, 0x00, 0xEB, 0xB5,
	0x90, 0x90
};

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

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C"
{
	__declspec(dllexport) void RF_ModuleInit(wchar_t* mod_path)
	{
		char* _equipOffset = SignatureScan<char*>("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x0F\xB6\x48\x02\x84\xC9\x74\x19", "xxxxx????xxxxxxxx");
		char* _listOffset = SignatureScan<char*>("\x48\x89\x5C\x24\x18\x57\x48\x83\xEC\x20\x33\xDB\x48\x89\x6C\x24\x30\x41\x8B\xF8\x48\x8B\xE9", "xxxxxxxxxxxxxxxxxxxxxxx");
		char* _iconOffset = SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xFA\x41\x0F\xB6\xD9\x41\x0F\xB6\xD0\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxx????");
		char* _categoryOffset = SignatureScan<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x80\x01\x00\x00\x33\xF6\x89\x4C\x24\x28\x85\xC9\x48\x8D\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxx????");

		char* _allocMemory = (char*)malloc(0x19);

		memcpy(_allocMemory, _iconOffset + 0x1D, 0x19);

		memcpy(_iconOffset + 0x1C, _allocMemory, 0x19);
		memcpy(_iconOffset + 0x1A, _instShortcut.data(), 0x02);

		memcpy(_iconOffset + 0x35, _instShortcut.data() + 0x02, 0x05);
		memcpy(_iconOffset + 0x3A, _instShortcut.data() + 0x0C, 0x01);
		memcpy(_iconOffset + 0x3B, _instShortcut.data() + 0x07, 0x05);

		memcpy(_listOffset + 0x18E, _instShortcut.data() + 0x0D, 0x02);
		memcpy(_listOffset + 0x138, _instShortcut.data() + 0x0F, 0x04);
		memcpy(_listOffset + 0x188, _instShortcut.data() + 0x13, 0x06);

		memcpy(_listOffset + 0x12C, _instShortcut.data() + 0x2C, 0x04);
		memcpy(_listOffset + 0x173, _instShortcut.data() + 0x30, 0x08);

		memcpy(_equipOffset + 0x33, _instShortcut.data() + 0x19, 0x05);
		memcpy(_equipOffset + 0x16, _instShortcut.data() + 0x1E, 0x07);
		memcpy(_equipOffset + 0x38, _instShortcut.data() + 0x25, 0x07);

		memcpy(_categoryOffset + 0x4AF, _instShortcut.data() + 0x38, 0x02);
	}
}