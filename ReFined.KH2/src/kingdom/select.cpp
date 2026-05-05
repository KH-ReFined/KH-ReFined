#include "select.h"

Tz::Select::SetCurrent_t Tz::Select::SetCurrent = SignatureScan<Tz::Select::SetCurrent_t>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\x66\x89\x11\x0F\xB7\x49\x04", "xxxxxxxxxxxxxxxxxxxxxxxxx");
Tz::Select::SetSelectTop_t Tz::Select::SetSelectTop = SignatureScan<Tz::Select::SetSelectTop_t>("\x4C\x8B\xC9\x66\x89\x51\x12\x4C\x8D\x41\x12\x0F\xB7\xCA\x41\x80\x79\x25\x00", "xxxxxxxxxxxxxxxxxxx");
Tz::Select::SetItem_t Tz::Select::SetItem = SignatureScan<Tz::Select::SetItem_t>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x40\x48\x8B\x9C\x24\x80\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx");
Tz::Select::GetItemPtr_t Tz::Select::GetItemPtr = SignatureScan<Tz::Select::GetItemPtr_t>("\x48\x8B\x41\x28\x48\x63\xD2\x48\x8B\x04\xD0\xC3", "xxxxxxxxxxxx");
Tz::Select::GetSelectPos_t Tz::Select::GetSelectPos = SignatureScan<Tz::Select::GetSelectPos_t>("\x0F\xB7\x51\x16\x0F\xBF\x41\x12\x0F\xBF\x09\x03\xC1\x66\x85\xD2\x74\x08\x0F\xBF\xCA\x99\xF7\xF9\x8B\xC2\xC3", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
Tz::Select::SetExOffset_t Tz::Select::SetExOffset = SignatureScan<Tz::Select::SetExOffset_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x48\x8B\x41\x28", "xxxxxxxxxxxxxxxxxxxxxxxx");
Tz::Select::SetPageMode_t Tz::Select::SetPageMode = SignatureScan<Tz::Select::SetPageMode_t>("\x44\x8B\x41\x40\x41\x8B\xC0\x25\xFF\xCF\xFF\xFF\x88\x51\x27\x41", "xxxxxxxxxxxxxxxx");
Tz::Select::SetSelectMax_t Tz::Select::SetSelectMax = SignatureScan<Tz::Select::SetSelectMax_t>("\x85\xD2\x79\x09\x0F\xB7\x41\x04\x66\x89\x41\x16\xC3\x66\x89\x51", "xxxxxxxxxxxxxxxx");