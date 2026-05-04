#include "partyinfo.h"

Tz::PartyInfo::GetName_t Tz::PartyInfo::GetName = SignatureScan<Tz::PartyInfo::GetName_t>("\x48\x63\xC2\x48\xC1\xE0\x05\x48\x8B\x44\x08\x10\xC3", "xxxxxxxxxxxxx");
Tz::PartyInfo::GetSheet_t Tz::PartyInfo::GetSheet = SignatureScan<Tz::PartyInfo::GetSheet_t>("\x48\x63\xC2\x48\xFF\xC0\x48\xC1\xE0\x05\x48\x8B\x04\x08\xC3", "xxxxxxxxxxxxxxx");
