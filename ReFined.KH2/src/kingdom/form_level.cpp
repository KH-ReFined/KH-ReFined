#include "form_level.h"
#include "SigScan.h"

YS::FORM_LEVEL::Search_t YS::FORM_LEVEL::Search = SignatureScan<YS::FORM_LEVEL::Search_t>("\x48\x83\xEC\x38\x48\x63\xC2\x41\xB9\x08\x00\x00\x00", "xxxxxxxxxxxxx");
YS::FORM_LEVEL::GetSummonTable_t YS::FORM_LEVEL::GetSummonTable = reinterpret_cast<YS::FORM_LEVEL::GetSummonTable_t>(SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x8B\x15\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x33\xDB\x48", "xxxxxxxxxxxxxxxxxx????xxx????xxx") + 0xC0);