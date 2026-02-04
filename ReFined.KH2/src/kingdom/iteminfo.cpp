#include "iteminfo.h"

Tz::ItemInfo::SetItemInfo_t Tz::ItemInfo::SetItemInfo = SignatureScan<Tz::ItemInfo::SetItemInfo_t>("\x40\x53\x56\x57\x41\x54\x41\x56\x48\x83\xEC\x20\x4C\x8D\x71\x04", "xxxxxxxxxxxxxxxx");
Tz::ItemInfo::MakeMsgTbl_t Tz::ItemInfo::MakeMsgTbl = SignatureScan<Tz::ItemInfo::MakeMsgTbl_t>("\x40\x53\x55\x56\x57\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x20\x48", "xxxxxxxxxxxxxxxx");
