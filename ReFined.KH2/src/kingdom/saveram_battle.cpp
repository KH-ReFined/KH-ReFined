#include "saveram_battle.h"

YS::SAVERAM_BATTLE::inc_kill_count_t YS::SAVERAM_BATTLE::inc_kill_count = SignatureScan<YS::SAVERAM_BATTLE::inc_kill_count_t>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xDA\x48\x8B\xF9\xE8\x00\x00\x00\x00\x84\xC0\x74\x42\x8D\x83\x18\xFC\xFF\xFF", "xxxxxxxxxxxxxxxx????xxxxxxxxxx");