#include "cmtop.h"

int (*Tz::CmTop::GetCurPos)(int) = ResolveFunctionFromCall<int(*)(int)>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x01D5);
int (*Tz::CmTop::GetSelectPos)(int) = SignatureScan<int(*)(int)>("\x48\x63\xC1\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x0C\x00\x0F\xBF\x44\x11\x4C\x0F\xBF\x0C\x11\x03\xC1\xC3", "xxxxxx????xxxxxxxxxxxxxxxx");
char** (*Tz::CmTop::GetListBuffer)() = ResolveFunctionFromCall<char** (*)()>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x000E);
char* (*Tz::CmTop::GetTopPlateSeqTbl)(int) = ResolveFunctionFromCall<char* (*)(int)>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x007C);
int (*Tz::CmTop::GetFontColorSeqNum)(int) = ResolveRelativeAddress<int(*)(int)>(ResolveRelativeAddress<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x0076);
int (*Tz::CmTop::GetItemFontColor)(int, int, bool) = ResolveRelativeAddress<int(*)(int, int, bool)>(ResolveRelativeAddress<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x01E6);
char* (*Tz::CmTop::GetScrBarSeqTbl)() = ResolveFunctionFromCall<char*(*)()>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", 0x475);

char** Tz::CmTop::m_Lay = ResolveRelativeAddress<char**>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x00B0);
char** Tz::CmTop::m_MenuPtr = ResolveRelativeAddress<char**>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x009F);
char** Tz::CmTop::m_SclBar = ResolveRelativeAddress<char**>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", 0x484);
int* Tz::CmTop::m_SeqUnit = ResolveRelativeAddress<int*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0098);
int* Tz::CmTop::m_ImgUnit = ResolveRelativeAddress<int*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x008E);
int* Tz::CmTop::m_DummySeq = ResolveRelativeAddress<int*>(ResolveRelativeAddress<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F), 0x0094);
char** Tz::CmTop::m_ItemInfo = ResolveRelativeAddress<char**>("\x40\x53\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x28\x48\x89\x7C\x24\x60\x4C\x89\x74\x24\x20\xE8", "xxxxxxxxxxxxxxxxxxxxxxx", 0x0104);
